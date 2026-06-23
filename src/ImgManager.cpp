#include <stdio.h>
#include <string.h>

#include <windows.h>

#include <d2d1.h>
#include <d2d1helper.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "ImgManager.hpp"

char resPath[MAX_PATH_LEN];
unsigned char* currentScene = NULL;
// 릴리즈 필요 > 메모리 누수 (재활용 객체)
ID2D1HwndRenderTarget* renderTarget = nullptr;
// 릴리즈 필요 > 메모리 누수 (재활용 객체)
ID2D1Bitmap* nowScene = nullptr;
ID2D1Bitmap* nextScene = nullptr;

D2D1_BITMAP_PROPERTIES props =
        D2D1::BitmapProperties(
            D2D1::PixelFormat(
                DXGI_FORMAT_R8G8B8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED
            )
        );

void GetResourceDir() {
    GetCurrentDirectoryA(MAX_PATH_LEN, resPath);
    strcat_s(resPath, MAX_PATH_LEN, "\\res");
}

void LoadScene(unsigned char *file_name) {
    char file_path[MAX_PATH_LEN] = {0};
    
    sprintf_s(file_path, MAX_PATH_LEN, "%s\\%s", resPath, file_name);

    int imgWidth, imgHeight, imgChannels;

    unsigned char *data = stbi_load(file_path, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);

    if (data) {
        printf("Load Success, H:%d, W:%d, C:%d", imgHeight, imgWidth, imgChannels);
        currentScene = data;
    }
    else {
        printf("Load Error");
        currentScene = NULL;
    }
}

void D2DFactoryInit(HWND hwnd, D2D1_SIZE_U size) {
    // 릴리즈 필요 > 메모리 누수 (재활용 객체)
    ID2D1Factory* D2DFactory = nullptr;

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2DFactory);

    D2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, size),
        &renderTarget
    );
}

void D2DSceneDraw(float sceneAlpha) {
    renderTarget->BeginDraw();

    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    if (nowScene) {
        renderTarget->DrawBitmap(nowScene, nullptr, sceneAlpha);
    }

    renderTarget->EndDraw();
}

void CacheScene() {
    if (!currentScene) return;

    HRESULT hr = renderTarget->CreateBitmap(
        D2D1::SizeU(1280, 720),
        currentScene,
        1280 * 4,
        &props,
        &nowScene
    );

    stbi_image_free(currentScene);
    currentScene = nullptr;
}

void RenderProduction() {
    LoadScene((unsigned char*)"production.png");
    CacheScene();
    float sceneAlpha = 0.0f;

    while (sceneAlpha < 1.0f) {
        D2DSceneDraw(sceneAlpha += 0.05f);
        Sleep(10);
    }

    Sleep(500);

    while (sceneAlpha > 0.0f) {
        D2DSceneDraw(sceneAlpha -= 0.05f);
        Sleep(10);
    }

    Sleep(500);
}

// TODO: implement preloaded image que (circular)