#include <stdio.h>
#include <string.h>

#include <windows.h>

#include <d2d1.h>
#include <d2d1helper.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "ImgManager.hpp"

char resPath[MAX_PATH_LEN];

// 릴리즈 필요 > 메모리 누수 (재활용 객체)
ID2D1HwndRenderTarget* renderTarget = nullptr;

// Scene Components
ImgDataStruct currentSceneData;

// 릴리즈 필요 > 메모리 누수 (재활용 객체)
ID2D1Bitmap* currentSceneCache = nullptr;

// Button Components
ImgDataStruct componentData;

ID2D1Bitmap* componentCache = nullptr; 

D2D1_BITMAP_PROPERTIES props =
        D2D1::BitmapProperties(
            D2D1::PixelFormat(
                DXGI_FORMAT_R8G8B8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED
            )
        );

/**
 * @brief 윈도우 라이브러리를 이용해 현재 리소스 디렉토리를 resPath 변수에 저장한다.
 * **/
void GetResourceDir() {
    GetCurrentDirectoryA(MAX_PATH_LEN, resPath);
    strcat_s(resPath, MAX_PATH_LEN, "\\res");
}

/**
 * @brief stbi 라이브러리를 이용해 이미지를 로드한다.
 * @param file_name 로드할 파일의 이름
 * @param dst 파일의 정보를 저장할 구조체 포인터
 * @param chanel stbi_load의 파라미터, 로드할 이미지의 채널을 입력받는다. 본 프로젝트에서는 STBI_rgb_alpha(4채널 RGBA)를 디폴트로 한다.
 * **/
void LoadImgFile(unsigned char *file_name, ImgDataStruct* dst, int chanel) {
    char file_path[MAX_PATH_LEN] = {0};
    
    sprintf_s(file_path, MAX_PATH_LEN, "%s\\%s", resPath, file_name);

    int imgWidth, imgHeight, imgChannels;

    unsigned char *data = stbi_load(file_path, &imgWidth, &imgHeight, &imgChannels, chanel);

    if (data) {
        printf("Load Success, H:%d, W:%d, C:%d", imgHeight, imgWidth, imgChannels);
        dst->ImgData = data;
        dst->imgWidth = imgWidth;
        dst->imgHeight = imgHeight;
        dst->imgChannels = imgChannels;
    }
    else {
        printf("Load Error");
        dst->ImgData = NULL;
    }
}

/**
 * @brief D2DFactory COM 객체를 생성하고 HwndRenderTarget을 초기화한다. (임시: D2DFactory 객체를 전역으로 돌려서 lifecycle 관리를 해줘야 함.)
 * 
 * @note 현재 D2DFactory의 수명 관리가 함수 내부에 종속되어 있음.
 *       추후 멤버 변수 또는 전역 객체로 관리하여 Release 시점을 제어해야 한다.
 * 
 * @param hwnd 렌더링 대상 윈도우 핸들
 * @param size 렌더링 대상 영역 크기
 * **/
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

/**
 * @brief Direct2D를 이용해 대상 비트맵을 화면에 그린다.
 * @param src 렌더링 대상 비트맵
 * @param sceneAlpha 대상 비트맵의 투명도
 * **/
void D2DDrawImg(ID2D1Bitmap* src, float sceneAlpha) {
    renderTarget->BeginDraw();

    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    if (src) {
        renderTarget->DrawBitmap(src, nullptr, sceneAlpha);
    }

    renderTarget->EndDraw();
}

/**
 * @brief 이미지 데이터를 Direct 2D 비트맵으로 캐싱한다. 원본 이미지 데이터는 release 된다.
 * @param src 원본 이미지 데이터 구조체
 * @param dst 생성된 비트맵 객체를 저장할 출력 포인터
 * **/
void CacheImg(ImgDataStruct* src, ID2D1Bitmap** dst) {
    if (!src->ImgData) return;

    HRESULT hr = renderTarget->CreateBitmap(
        D2D1::SizeU(src->imgWidth, src->imgHeight),
        src->ImgData,
        src->imgWidth * src->imgChannels,
        &props,
        dst
    );

    stbi_image_free(src->ImgData);
    src->ImgData = nullptr;
}

/**
 * @brief 프로덕션 이미지 및 타이틀을 화면에 출력한다.
 * @param state 게임의 씬 관리를 위한 상태 머신
 * **/
void LoadTitle(int* state) {
    if (*state == 0)
    {
        LoadImgFile((unsigned char*)"production.png", &currentSceneData, STBI_rgb_alpha);
        CacheImg(&currentSceneData, &currentSceneCache);
        float sceneAlpha = 0.0f;

        while (sceneAlpha < 1.0f) {
            D2DDrawImg(currentSceneCache, sceneAlpha += 0.05f);
            Sleep(10);
        }

        Sleep(500);

        while (sceneAlpha > 0.0f) {
            D2DDrawImg(currentSceneCache, sceneAlpha -= 0.05f);
            Sleep(10);
        }

        Sleep(500);

        (*state)++;
        
        LoadImgFile((unsigned char*)"title.png", &currentSceneData, STBI_rgb_alpha);
        CacheImg(&currentSceneData, &currentSceneCache);
    }
    D2DDrawImg(currentSceneCache, 1.0f);
}

void RenderComponent() {
    LoadImgFile((unsigned char*)"gameStartBtn.png", &componentData, STBI_rgb_alpha);
    CacheImg(&componentData, &componentCache);
    D2DDrawImg(componentCache, 1.0f);
}

// TODO: implement preloaded image que (circular)