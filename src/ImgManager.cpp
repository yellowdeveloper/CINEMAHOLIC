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
 * @param src 렌더링 대상 비트맵
 * @param sceneAlpha 대상 비트맵의 투명도
 * **/
void RenderAllComponents(RenderData* srcArr, int objectCount) {
    renderTarget->BeginDraw();

    renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    for (int i = 0; i < objectCount ; i++) {

        D2D1_RECT_F rect = D2D1::RectF(
            srcArr[i].position.x,
            srcArr[i].position.y,
            srcArr[i].position.x + srcArr[i].position.width,
            srcArr[i].position.y + srcArr[i].position.height
        ); 

        renderTarget->DrawBitmap(srcArr[i].sprite.ImgCache, rect, srcArr[i].opacity);
    }

    renderTarget->EndDraw();
}

/**
 * @brief 이미지 데이터를 Direct 2D 비트맵으로 캐싱한다. 원본 이미지 데이터는 release 된다.
 * @param src 원본 이미지 데이터 구조체
 * @param dst 생성된 비트맵 객체를 저장할 출력 포인터
 * **/
void LoadAndCacheImg(unsigned char *file_name, int chanel, RenderData* dst) {
    char file_path[MAX_PATH_LEN] = {0};
    
    sprintf_s(file_path, MAX_PATH_LEN, "%s\\%s", resPath, file_name);

    int imgWidth, imgHeight, imgChannels;

    unsigned char *data = stbi_load(file_path, &imgWidth, &imgHeight, &imgChannels, chanel);

    if (!data) return;

    HRESULT hr = renderTarget->CreateBitmap(
        D2D1::SizeU(imgWidth, imgHeight),
        data,
        imgWidth * imgChannels,
        &props,
        &(dst->sprite.ImgCache)
    );

    dst->size.imgWidth    = imgWidth;
    dst->size.imgHeight   = imgHeight;
    dst->size.imgChannels = imgChannels;

    stbi_image_free(data);
}

// TODO: implement preloaded image que (circular)