#ifndef ImgMGR_H
#define ImgMGR_H

#define MAX_RENDER_NUM 1024
#define MAX_PATH_LEN 256

extern char resPath[MAX_PATH_LEN];
extern ID2D1HwndRenderTarget* renderTarget;

void GetResourceDir();
void LoadAndCacheImg(unsigned char *file_name, int chanel, RenderData* dst);
void D2DFactoryInit(HWND hwnd, D2D1_SIZE_U size);
void RenderAllComponents(RenderData* srcArr, int objectCount);

#endif