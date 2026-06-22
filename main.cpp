// header includes
// standard headers
#include <stdio.h>
#include <string.h>
// win api
#include <windows.h>
#include <wincodec.h>
// directx
#include <d2d1.h>
#include <d2d1helper.h>
// incl
#include <ImgManager.hpp>

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
    const wchar_t CLASS_NAME[] = L"CINEMAHOLIC";
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"CINEMA HOLIC",
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

    ShowWindow(hwnd, nCmdShow);

    // Initialize Component Object Model
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    GetResourceDir();
    CustomLoadImage(L"title.png");

    MSG msg = {0};

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // If press "x" btn, exit window.
    // Uninitialize Component Object Model
    CoUninitialize();
    return 0;
}