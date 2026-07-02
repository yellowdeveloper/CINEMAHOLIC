// header includes
// standard headers
#include <stdio.h>
#include <string.h>
#include <time.h>
// win api
#include <windows.h>
// directx
#include <d2d1.h>
#include <d2d1helper.h>
// C++ lib
#include <thread>
// incl
#include "structs.h"
#include "ImgManager.hpp"
#include "ObjectController.hpp"
#include "Scenes.hpp"
#include "Events.hpp"

#define MAX_COMPONENT_NUM 1024
#define MAX_FPS 60
#define FRAME_TIME float(1000.0f / MAX_FPS)

// 0: init
// 1: title
// 2: game - tycoon
// 3: game - visual novel
// 4: game - minigame
// 5: game - ending
// 6: shut down
int game_state = 0;

int mouseX = 0;
int mouseY = 0;

bool mouseLButtonPressed = false;
bool mouseRButtonPressed = false;

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
RenderData RenderBuff_A[MAX_COMPONENT_NUM];
RenderData RenderBuff_B[MAX_COMPONENT_NUM];

// 비트맵 캐시 관리 :: 배열
// 이미지 미리 로드
Sprite CacheArr[MAX_COMPONENT_NUM];

// 게임 프레임 레이트 관리
clock_t start = clock();

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
            mouseLButtonPressed = true;
            return 0;
        }

        case WM_LBUTTONUP : {
            mouseLButtonPressed = false;
            return 0;
        }

        case WM_RBUTTONDOWN : {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            mouseRButtonPressed = true;
            return 0;
        }

        case WM_RBUTTONUP : {
            mouseRButtonPressed = false;
            return 0;
        }

        case WM_MOUSEMOVE : {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            mouseX = x;
            mouseY = y;
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

    RECT rc = { 0, 0, 1280, 720 };

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

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    GetResourceDir();
    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
    D2DFactoryInit(hwnd, size);    

    // Show Production Logo And Change State to title

    // Run Game Logic Thread
    std::thread update(UpdateThread, ComponentsArr, MAX_COMPONENT_NUM);
    // TODO: Run Asset Pre-Loadder Thread

    MSG msg = {0};
    boolean running = true;

    LARGE_INTEGER frequency, start, end;
    double deltaTime = 0.0f;

    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    QueryPerformanceCounter(&start);

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

        if (deltaTime < FRAME_TIME) {
            QueryPerformanceCounter(&end);
            deltaTime = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart * 1000.0f;
            continue;
        }

        QueryPerformanceCounter(&start);

        if (updateState == LOADING) {
            ClearComponents(ComponentsArr, MAX_COMPONENT_NUM);
            ClearCache(CacheArr, MAX_COMPONENT_NUM);

            LoadingScene(nextScene, &game_state, ComponentsArr, RenderBuff_A, CacheArr);

            continue;
        }

        if (currentScene) 
            currentScene(&game_state, ComponentsArr, RenderBuff_A, CacheArr);
    }

    updateState = EXIT;
    SetEvent(g_UpdateEvent);

    if (update.joinable()) {
        update.join();
    }

    return 0;
}