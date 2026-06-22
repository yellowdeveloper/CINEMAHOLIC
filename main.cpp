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

    ShowWindow(hwnd, nCmdShow);

    // TODO: Run Game Logic Thread
    // TODO: Run Asset Pre-Loadder Thread
    // Under this cite will be in the Asset Pre-Loader
    GetResourceDir();
    CustomLoadResource((unsigned char*)"title.png");

    ID2D1Factory* D2DFactory = nullptr;
    ID2D1HwndRenderTarget* renderTarget = nullptr;

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory);

    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    D2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, size),
        &renderTarget
    );

    ID2D1Bitmap* bitmap = nullptr;

    D2D1_BITMAP_PROPERTIES props =
        D2D1::BitmapProperties(
            D2D1::PixelFormat(
                DXGI_FORMAT_R8G8B8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED
            )
        );

    HRESULT hr = renderTarget->CreateBitmap(
        D2D1::SizeU(1280, 720),
        preloaded,
        1280 * 4,
        &props,
        &bitmap
    );

    renderTarget->BeginDraw();

    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    if (bitmap) {
        renderTarget->DrawBitmap(bitmap);
    }

    renderTarget->EndDraw();

    MSG msg = {0};

    while(GetMessage(&msg, NULL, 0, 0)) {
        // Main Thread Will Process Window, Image Render
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}