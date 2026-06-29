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
#include "ObjectController.hpp"

#define MAX_COMPONENT_NUM 1024

// 0: loading
// 1: title
// 2: game - tycoon
// 3: game - visual novel
// 4: game - minigame
// 5: game - ending
// 6: shut down
int game_state = 0;

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

void LoadTitle() {
    D2D1_SIZE_U size;

    if (!game_state){
        RenderData productionPage;
        LoadAndCacheImg((unsigned char *)"production.png", 4, &CacheArr[0]);
        productionPage.spriteID = 0;

        float op = 0.0f;

        size = CacheArr[productionPage.spriteID].ImgCache->GetPixelSize();
        
        productionPage.position.x = 0;
        productionPage.position.y = 0;
        productionPage.position.width = size.width;
        productionPage.position.height = size.height;

        while(op < 1.0f) {
            productionPage.opacity = op;
            RenderBuff_A[0] = productionPage;

            RenderAllComponents(RenderBuff_A, CacheArr, 1);

            op += 0.05f;

            Sleep(10);
        }

        LoadAndCacheImg((unsigned char *)"title.png", 4, &CacheArr[1]);
        LoadAndCacheImg((unsigned char *)"gameStartBtn.png", 4, &CacheArr[2]);
        LoadAndCacheImg((unsigned char *)"gameLoadBtn.png", 4, &CacheArr[3]);

        while(op > 0.0f) {
            productionPage.opacity = op;
            RenderBuff_A[0] = productionPage;

            RenderAllComponents(RenderBuff_A, CacheArr, 1);

            op -= 0.05f;
            
            Sleep(10);
        }
        
        CacheArr[0].Release();

        game_state++;
    }
    RenderData titleImg;

    // Component no.0 = start button
    ComponentsArr[0].spriteID = 2;
    ComponentsArr[0].enabled = true;

    size = CacheArr[ComponentsArr[0].spriteID].ImgCache->GetPixelSize();
    ComponentsArr[0].position.x = 530;
    ComponentsArr[0].position.y = 440;
    ComponentsArr[0].position.width = size.width;
    ComponentsArr[0].position.height = size.height;
    ComponentsArr[0].opacity = 1.0f;

    // Component no.1 = load button
    ComponentsArr[1].spriteID = 3;
    ComponentsArr[1].enabled = true;

    size = CacheArr[ComponentsArr[1].spriteID].ImgCache->GetPixelSize();
    ComponentsArr[1].position.x = 530;
    ComponentsArr[1].position.y = 560;
    ComponentsArr[1].position.width = size.width;
    ComponentsArr[1].position.height = size.height;
    ComponentsArr[1].opacity = 1.0f;
    ComponentsArr[1].update1 = ButtonHover;

    titleImg.spriteID = 1;
    size = CacheArr[titleImg.spriteID].ImgCache->GetPixelSize();

    titleImg.position.x = 0;
    titleImg.position.y = 0;
    titleImg.position.width = size.width;
    titleImg.position.height = size.height;
    titleImg.opacity = 1.0f;

    RenderBuff_A[0] = titleImg;

    RenderBuff_A[1].enabled = true;
    RenderBuff_A[1].spriteID = ComponentsArr[0].spriteID;
    RenderBuff_A[1].position = ComponentsArr[0].position;
    RenderBuff_A[1].opacity  = ComponentsArr[0].opacity;

    RenderBuff_A[2].enabled = true;
    RenderBuff_A[2].spriteID = ComponentsArr[1].spriteID;
    RenderBuff_A[2].position = ComponentsArr[1].position;
    RenderBuff_A[2].opacity  = ComponentsArr[1].opacity;

    RenderAllComponents(RenderBuff_A, CacheArr, 3);
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