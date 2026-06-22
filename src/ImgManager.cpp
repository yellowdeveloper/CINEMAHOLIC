#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <wincodec.h>

#include "ImgManager.hpp"

wchar_t res_path[256];

void GetResourceDir() {
    GetCurrentDirectoryW(sizeof(res_path), res_path);
    wcscat_s(res_path, sizeof(res_path), L"\\res\\");
}

IWICBitmapFrameDecode* CustomLoadImage(wchar_t* img_name) {
    wchar_t img_path[256] = L"";

    swprintf_s(img_path, L"%ls\\%ls", res_path, img_name);

    IWICImagingFactory* g_wicFactory = NULL;

    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&g_wicFactory)
    );

    IWICBitmapDecoder* decoder = NULL;

    hr = g_wicFactory->CreateDecoderFromFilename(
            img_path,
            NULL,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &decoder
        );

    if (FAILED(hr))
        return NULL;

    IWICBitmapFrameDecode* frame = NULL;

    hr = decoder->GetFrame(
        0,
        &frame
    );

    decoder->Release();

    if (FAILED(hr))
        return NULL;

    return frame;
}