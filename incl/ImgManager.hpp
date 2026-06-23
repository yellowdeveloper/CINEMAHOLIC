#ifndef ImgMGR_H
#define ImgMGR_H

#define MAX_PATH_LEN 256

extern char resPath[MAX_PATH_LEN];
extern unsigned char* currentScene;
extern ID2D1HwndRenderTarget* renderTarget;

void GetResourceDir();
void LoadScene(unsigned char *file_name);
void D2DFactoryInit(HWND hwnd, D2D1_SIZE_U size);
void D2DSceneDraw(float sceneAlpha);
void CacheScene();
void RenderProduction();

#endif