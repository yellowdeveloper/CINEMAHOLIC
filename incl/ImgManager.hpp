#ifndef ImgMGR_H
#define ImgMGR_H

#define MAX_PATH_LEN 256

extern char resPath[MAX_PATH_LEN];
extern ID2D1HwndRenderTarget* renderTarget;

void GetResourceDir();
void LoadAndCacheImg(unsigned char *file_name, int chanel, Sprite* dst);
void D2DFactoryInit(HWND hwnd, D2D1_SIZE_U size);
void RenderAllComponents(RenderData* srcArr, Sprite* srcSpriteArr,int objectCount);

#endif