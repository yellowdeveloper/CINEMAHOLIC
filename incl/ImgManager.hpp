#ifndef ImgMGR_H
#define ImgMGR_H

#define MAX_PATH_LEN 256

extern char resPath[MAX_PATH_LEN];
extern ID2D1HwndRenderTarget* renderTarget;

void GetResourceDir();
void LoadAndCacheImg(unsigned char *file_name, int chanel, Sprite* dst);
void D2DFactoryInit(HWND hwnd, D2D1_SIZE_U size);
void RenderAllComponents(RenderData* srcArr, Sprite* srcSpriteArr,int objectCount);
void ClearCache(Sprite* cacheArr, size_t max_cache);
void RenderLoadingAnimation(D2D1::ColorF color, int count);
void RenderSingleSprite(Sprite srcSprite, float x, float y, float opacity);
void MatchRenderData(RenderData* dst, ComponentData* src);
void SetRenderData(RenderData* dst, int spriteID, Position pos, float opacity);

#endif