// header includes
// standard headers
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
// win api
#include <windows.h>
// directx
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
// C++ lib
#include <thread>
// incl
#include "structs.h"
#include "ImgManager.hpp"
#include "ObjectController.hpp"
#include "InputManager.hpp"
#include "Scenes.hpp"
#include "AudioManager.hpp"
#include "TextManager.hpp"

#ifndef QPC
// build.bat overrides this globally: enabled for debug, disabled for release.
#define QPC 1
#endif

#if QPC
#include "QPC_MemBuf.hpp"
#endif

#define MAX_FPS 60
#define FRAME_TIME float(1000.0f / MAX_FPS)

// State Management (Game State)
GameState game_state = INIT;

InputEv inputEV;

HANDLE g_UpdateEvent = CreateEvent(
    NULL,   // 보안
    TRUE,   // Manual Reset
    TRUE,   // 초기 상태: Signaled(실행)
    NULL
);
UpdateState updateState = PROCESSING;
SceneFunc currentScene = LoadTitle;
SceneFunc nextScene = nullptr;

// 컴포넌트 관리 :: 배열
ComponentData ComponentsArr[MAX_COMPONENT_NUM];

// 이미지 렌더링 :: 더블 버퍼
// 로직 스레드 > 렌더 스냅샷 write
// 렌더 타임에 buffer swap > 렌더링 진행
RenderContext renderContext;

RenderData RenderBuff_A[MAX_COMPONENT_NUM];
RenderData RenderBuff_B[MAX_COMPONENT_NUM];

// 비트맵 캐시 관리 :: 배열
// 이미지 미리 로드
Sprite CacheArr[MAX_COMPONENT_NUM];

// 게임 프레임 레이트 관리
clock_t start = clock();
double fps = 0.0f;
double d_time_sync = 0.0f;

std::mutex tickMutex;
std::condition_variable tickCv;

unsigned pendingTicks = 0;
bool stopping = false;

// Window Procedure : CALLBACK Function = Process Messages From OS
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        // if receive WM_DESTROY MSG : send signal to end message loop
        case WM_DESTROY :
            PostQuitMessage(0);
            return 0;

        // if receive WM_PAINT MSG : Draw Window
        case WM_PAINT : {
        // Get Paint Struct from Device Context and Draw Window
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN : {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            inputEV.mouseLButtonPressed = true;
            return 0;
        }

        case WM_LBUTTONUP : {
            inputEV.mouseLButtonPressed = false;
            return 0;
        }

        case WM_RBUTTONDOWN : {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            inputEV.mouseRButtonPressed = true;
            return 0;
        }

        case WM_RBUTTONUP : {
            inputEV.mouseRButtonPressed = false;
            return 0;
        }

        case WM_MOUSEMOVE : {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            inputEV.mouseX = x;
            inputEV.mouseY = y;
            return 0;
        }

        case WM_KEYDOWN : {
            inputEV.keyStateArr[wParam] = PRESSED;
            return 0;
        }

        case WM_KEYUP : {
            inputEV.keyStateArr[wParam] = RELEASED;
            return 0;
        }

        default :
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){
    const char CLASS_NAME[] = "CINEMAHOLIC";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    RECT rc = { 0, 0, SCREEN_HD_W, SCREEN_HD_H };

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "CINEMA HOLIC",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    // resource directory init
    GetResourceDir();

    renderContext.updateBuffer = RenderBuff_A;
    renderContext.renderBuffer = RenderBuff_B;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
    D2DFactoryInit(hwnd, size);
    InitWriteFactoryFormat(); 

#if QPC
    QpcMemoryLogger(1000000);
#endif

    // Show Production Logo And Change State to title

    // Run Game Logic Thread
    std::thread update(UpdateThread, ComponentsArr, &renderContext);
    // TODO: Run Asset Pre-Loadder Thread

    MSG msg = {0};
    boolean running = true;

    LARGE_INTEGER frequency, start, end, updateLast;
    double deltaTime = 0.0f;
    double updateMs = 0.0;

    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    QueryPerformanceCounter(&start);

    currentScene(&game_state, ComponentsArr, CacheArr);
    QueryPerformanceCounter(&updateLast);

    while(running) {
        // Main Thread Will Process Window, Image Render
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = false;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (!running) break;
        }

        QueryPerformanceCounter(&end);
        deltaTime = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart * 1000.0f;
        d_time_sync = deltaTime;

        const double updateElapsedMs = (double)(end.QuadPart - updateLast.QuadPart) * 1000.0 / frequency.QuadPart;
        updateLast = end;

        if (updateState == PROCESSING) {
            updateMs += updateElapsedMs;
            unsigned ticksDue = 0;
            while (updateMs >= UPDATE_TIME) {
                updateMs -= UPDATE_TIME;
                ++ticksDue;
            }

            if (ticksDue > 0) {
                std::lock_guard<std::mutex> lock(tickMutex);
                pendingTicks += ticksDue;
                tickCv.notify_one();
            }
        } else {
            updateMs = 0.0;
        }

        if (deltaTime < FRAME_TIME) {
            continue;
        }
        
        fps = 1000.0f / d_time_sync;
        deltaTime = 0.0f;
        QueryPerformanceCounter(&start);
         
        if (updateState == LOADING) {
            ResetEvent(g_UpdateEvent);

            renderContext.lock.lock();

            ClearComponents(ComponentsArr, MAX_COMPONENT_NUM);
            ClearCache(CacheArr, MAX_COMPONENT_NUM);

            renderContext.renderCount = 0;

            LoadingScene(&currentScene, &game_state, ComponentsArr, CacheArr);

            renderContext.lock.unlock();

            continue;
        }

        if (currentScene)  {
#if QPC
            Record(QPC_RENDER_LOCK_REQUEST);
#endif
            renderContext.lock.lock();
#if QPC
            Record(QPC_RENDER_LOCK_ACQUIRED);
#endif

            RenderData* tmpSwap = renderContext.renderBuffer;
            renderContext.renderBuffer = renderContext.updateBuffer;
            renderContext.updateBuffer = tmpSwap;

#if QPC
            Record(QPC_RENDER_START, d_time_sync);
#endif
            RenderAllComponents(renderContext.renderBuffer, CacheArr, renderContext.renderCount);
#if QPC
            Record(QPC_RENDER_END, d_time_sync);
#endif

            renderContext.lock.unlock();
#if QPC
            Record(QPC_RENDER_LOCK_RELEASED);
#endif
        }
    }

    updateState = EXIT;
    SetEvent(g_UpdateEvent);

    if (update.joinable()) {
        update.join();
    }

#if QPC
    DumpQpcMemoryLogger();
#endif

    return 0;
}
