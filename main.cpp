// header includes
// standard headers
#include <stdio.h>
#include <string.h>
// win api
#include <windows.h>
// directx
#include <d2d1.h>
#include <d2d1helper.h>
// incl
#include "ImgManager.hpp"

// 0: loading
// 1: title
// 2: game - tycoon
// 3: game - visual novel
// 4: game - minigame
// 5: game - ending
// 6: shut down
int game_state = 0;

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
    RenderProduction();
    game_state++;

    // TODO: Run Game Logic Thread
    // TODO: Run Asset Pre-Loadder Thread

    MSG msg = {0};

    while(GetMessage(&msg, NULL, 0, 0)) {
        if (game_state == 1) {
            if (currentScene == nullptr) {
                LoadScene((unsigned char*)"title.png");
                CacheScene();
            }
            D2DSceneDraw(1.0f);
        }
        // Main Thread Will Process Window, Image Render
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}