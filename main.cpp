// header includes
// standard headers
#include <stdio.h>
#include <string.h>
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

// 0: loading
// 1: title
// 2: game - tycoon
// 3: game - visual novel
// 4: game - minigame
// 5: game - ending
// 6: shut down
int game_state = 0;

// 이미지 렌더링 :: 더블 버퍼
// 로직 스레드 > 렌더 스냅샷 write
// 렌더 타임에 buffer swap > 렌더링 진행
RenderData RenderBuff_A[MAX_RENDER_NUM];
RenderData RenderBuff_B[MAX_RENDER_NUM];

void LoadTitle() {
    if (!game_state){
        RenderData productionPage;
        LoadAndCacheImg((unsigned char *)"production.png", 4, &productionPage);

        float op = 0.0f;

        productionPage.position.x = 0;
        productionPage.position.y = 0;
        productionPage.position.width = productionPage.size.imgWidth;
        productionPage.position.height = productionPage.size.imgHeight;

        while(op < 1.0f) {
            productionPage.opacity = op;
            RenderBuff_A[0] = productionPage;

            RenderAllComponents(RenderBuff_A, 1);

            op += 0.05f;

            Sleep(10);
        }

        Sleep(500);

        while(op > 0.0f) {
            productionPage.opacity = op;
            RenderBuff_A[0] = productionPage;

            RenderAllComponents(RenderBuff_A, 1);

            op -= 0.05f;
            
            Sleep(10);
        }
        
        game_state++;
    }
    RenderData titleImg;
    RenderData startBtn;
    RenderData loadBtn;

    LoadAndCacheImg((unsigned char *)"title.png", 4, &titleImg);
    LoadAndCacheImg((unsigned char *)"gameStartBtn.png", 4, &startBtn);
    LoadAndCacheImg((unsigned char *)"gameLoadBtn.png", 4, &loadBtn);

    titleImg.position.x = 0;
    titleImg.position.y = 0;
    titleImg.position.width = titleImg.size.imgWidth;
    titleImg.position.height = titleImg.size.imgHeight;
    titleImg.opacity = 1.0f;

    startBtn.position.x = 530;
    startBtn.position.y = 440;
    startBtn.position.width = startBtn.size.imgWidth;
    startBtn.position.height = startBtn.size.imgHeight;
    startBtn.opacity = 1.0f;

    loadBtn.position.x = 530;
    loadBtn.position.y = 560;
    loadBtn.position.width = loadBtn.size.imgWidth;
    loadBtn.position.height = loadBtn.size.imgHeight;
    loadBtn.opacity = 1.0f;

    RenderBuff_A[0] = titleImg;
    RenderBuff_A[1] = startBtn;
    RenderBuff_A[2] = loadBtn;

    RenderAllComponents(RenderBuff_A, 3);
}

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
            return 0;
        }

        case WM_RBUTTONDOWN : {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
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
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "CINEMA HOLIC",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    GetResourceDir();
    RECT rc;
    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
    D2DFactoryInit(hwnd, size);

    ShowWindow(hwnd, nCmdShow);

    // Show Production Logo And Change State to title

    // TODO: Run Game Logic Thread
    // TODO: Run Asset Pre-Loadder Thread

    MSG msg = {0};
    boolean running = true;

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
        }
        LoadTitle();
    }
    return 0;
}