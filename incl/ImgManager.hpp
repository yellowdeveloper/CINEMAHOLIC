#ifndef ImgMGR_H
#define ImgMGR_H

#define MAX_PATH_LEN 256

struct ImgDataStruct {
    unsigned char* ImgData;
    int imgHeight;
    int imgWidth;
    int imgChannels;
};

extern char resPath[MAX_PATH_LEN];
extern ImgDataStruct currentSceneData;
extern ImgDataStruct componentData;
extern ID2D1HwndRenderTarget* renderTarget;

void GetResourceDir();
void LoadImgFile(unsigned char *file_name, ImgDataStruct* dst, int chanel);
void D2DFactoryInit(HWND hwnd, D2D1_SIZE_U size);
void D2DDrawImg(ID2D1Bitmap* src, float sceneAlpha);
void CacheImg(ImgDataStruct* src, ID2D1Bitmap** dst);
void LoadTitle(int* state);
void RenderComponent();

#endif