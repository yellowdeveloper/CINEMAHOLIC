#include <stdio.h>
#include <string.h>

#include <windows.h>

#include <d2d1.h>
#include <d2d1helper.h>

#define STB_IMAGE_IMPLEMENTATION

#include "structs.h"
#include "stb_image.h"
#include "ImgManager.hpp"

char resPath[MAX_PATH_LEN];

// 릴리즈 필요 > 메모리 누수 (재활용 객체)
ID2D1HwndRenderTarget* renderTarget = nullptr;

// 릴리즈 필요 > 메모리 누수 (재활용 객체)
// ID2D1Bitmap* ImgCache = nullptr;

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
 * @param srcArr 렌더링 정보 배열
 * @param srcSpriteArr 대상 비트맵 배열
 * @param objectCount 렌더링 배열에 담긴 객체 수
 * **/
void RenderAllComponents(RenderData* srcArr, Sprite* srcSpriteArr, int objectCount) {
    renderTarget->BeginDraw();

    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    for (int i = 0; i < objectCount ; i++) {
        if (!srcArr[i].enabled) continue;

        D2D1_RECT_F rect = D2D1::RectF(
            srcArr[i].position.x,
            srcArr[i].position.y,
            srcArr[i].position.x + srcArr[i].position.width,
            srcArr[i].position.y + srcArr[i].position.height
        ); 

        renderTarget->DrawBitmap(srcSpriteArr[srcArr[i].resourceID].ImgCache, rect, srcArr[i].opacity);
    }

    renderTarget->EndDraw();
}

/**
 * @brief 이미지 데이터를 Direct 2D 비트맵으로 캐싱한다. 원본 이미지 데이터는 release 된다.
 * @param file_name 원본 이미지 파일 이름
 * @param chanel 불러올 채널 수 기본적으로 4 (ABGR)
 * @param dst 캐싱할 Sprite 구조체 포인터
 * **/
void LoadAndCacheImg(unsigned char *fileName, int chanel, Sprite* dst) {
    char file_path[MAX_PATH_LEN] = {0};
    
    sprintf_s(file_path, MAX_PATH_LEN, "%s\\%s", resPath, fileName);

    int imgWidth, imgHeight, imgChannels;

    unsigned char *data = stbi_load(file_path, &imgWidth, &imgHeight, &imgChannels, chanel);

    if (!data) return;

    HRESULT hr = renderTarget->CreateBitmap(
        D2D1::SizeU(imgWidth, imgHeight),
        data,
        imgWidth * imgChannels,
        &props,
        &(dst->ImgCache)
    );

    stbi_image_free(data);
}

void ClearCache(Sprite* cacheArr, size_t max_cache) {
    for (size_t i = 0; i < max_cache; i++) {
        if (cacheArr[i].ImgCache) {
            cacheArr[i].Release();
        }
    }
}

void RenderLoadingAnimation(D2D1::ColorF color, int count) {
    D2D1_SIZE_F size = renderTarget->GetSize();

    int radius[3] = {1, 3, 5};
    
    D2D1_ELLIPSE ellipse1 = D2D1::Ellipse(D2D1::Point2F(size.width - 20.0f, size.height - 20.0f), radius[count%3], radius[count%3]);
    D2D1_ELLIPSE ellipse2 = D2D1::Ellipse(D2D1::Point2F(size.width - 40.0f, size.height - 20.0f), radius[(count+1)%3], radius[(count+1)%3]);
    D2D1_ELLIPSE ellipse3 = D2D1::Ellipse(D2D1::Point2F(size.width - 60.0f, size.height - 20.0f), radius[(count+2)%3], radius[(count+2)%3]);

    ID2D1SolidColorBrush* pWhiteBrush;

    HRESULT hr = renderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        &pWhiteBrush
    );
    
    renderTarget->BeginDraw();

    renderTarget->Clear(color);

    renderTarget->FillEllipse(ellipse1, pWhiteBrush);
    renderTarget->FillEllipse(ellipse2, pWhiteBrush);
    renderTarget->FillEllipse(ellipse3, pWhiteBrush);

    renderTarget->EndDraw();
    pWhiteBrush->Release();
}

void RenderSingleSprite(Sprite srcSprite, float x, float y, float opacity) {
    D2D1_RECT_F rect = D2D1::RectF(
        x,
        y,
        x + (float)srcSprite.ImgCache->GetPixelSize().width,
        y + (float)srcSprite.ImgCache->GetPixelSize().height
    );

    renderTarget->BeginDraw();

    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    renderTarget->DrawBitmap(srcSprite.ImgCache, rect, opacity);

    renderTarget->EndDraw();
}

/**
 * @brief
 * @param hexColor 생성할 단색 비트맵의 색상 (BGRA hex code)
 * @note 단색 비트맵은 RenderData 설정 시에 크기(width, height)를 직접 지정해줘야 합니다.
 */
ID2D1Bitmap* GetSolidSprite(UINT32 hexColor) {
    ID2D1Bitmap* bitmap = nullptr;

    renderTarget->CreateBitmap(
        D2D1::SizeU(1, 1),
        &hexColor,
        sizeof(UINT32),
        &props,
        &bitmap
    );

    return bitmap;
}

void MatchRenderData(RenderData* dst, ComponentData* src) {
    if (src->enabled) dst->enabled = true;
    else dst->enabled = false;

    dst->resourceID = src->resourceID;
    dst->position = src->position;
    dst->opacity = src->opacity;
}

void SetRenderData(RenderData* dst, int resourceID, Position pos, float opacity) {
    dst->enabled = true;

    dst->resourceID = resourceID;
    dst->position = pos;
    dst->opacity = opacity;
}
// TODO: implement preloaded image que (circular)

ID2D1Bitmap* GetRenderedText() { // get font, size, color, text
    ID2D1Bitmap* bitmap = nullptr;

    renderTarget->DrawText(
        L"Hello, World!",
        13,
        nullptr, // IDWriteTextFormat* pTextFormat
        D2D1::RectF(0, 0, 200, 50),
        nullptr // ID2D1Brush* pForegroundBrush
    );

    return bitmap;
}