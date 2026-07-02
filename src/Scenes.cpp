#include <windows.h>
#include <d2d1.h>

#include "structs.h"
#include "ImgManager.hpp"
#include "ObjectController.hpp"
#include "Events.hpp"

void NovelScene(int *game_state, ComponentData* ComponentsArr, RenderData* RenderBuff, Sprite* CacheArr) {
    int spriteID = 0;
    D2D1_SIZE_U size;

    if (updateState == LOADING) {    
        LoadAndCacheImg((unsigned char *)"title.png", 4, &CacheArr[0]);
        LoadAndCacheImg((unsigned char *)"character1_standing.png", 4, &CacheArr[1]);
        LoadAndCacheImg((unsigned char *)"bottom_panel.png", 4, &CacheArr[2]);
        LoadAndCacheImg((unsigned char *)"gameStartBtn.png", 4, &CacheArr[3]);
        LoadAndCacheImg((unsigned char *)"gameLoadBtn.png", 4, &CacheArr[4]);

        spriteID = 1;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], spriteID, {0.0f, 223.0f, (float)size.width, (float)size.height}, 1.0f, 0.5f);

        spriteID = 2;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], spriteID, {150.0f, 575.0f, (float)size.width, (float)size.height}, 1.0f, 0.5f);

        spriteID = 3;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[2], spriteID, {150.0f, 575.0f, (float)size.width, (float)size.height}, 1.0f, 0.5f);

        spriteID = 4;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[3], spriteID, {150.0f, 575.0f, (float)size.width, (float)size.height}, 1.0f, 0.5f);

        updateState = PROCESSING;

        *game_state = 3;

        return;
    }

    spriteID = 0;
    size = CacheArr[spriteID].ImgCache->GetPixelSize();
    SetRenderData(&RenderBuff[0], spriteID, {0.0f, 0.0f, (float)size.width, (float)size.height}, 1.0f);

    MatchRenderData(&RenderBuff[1], &ComponentsArr[0]);
    MatchRenderData(&RenderBuff[2], &ComponentsArr[1]);
    MatchRenderData(&RenderBuff[3], &ComponentsArr[2]);
    MatchRenderData(&RenderBuff[4], &ComponentsArr[3]);

    RenderAllComponents(RenderBuff, CacheArr, 5);
}

// Title Screen : Scene number 0 ~ 1
void LoadTitle(int *game_state, ComponentData* ComponentsArr, RenderData* RenderBuff, Sprite* CacheArr) {
    D2D1_SIZE_U size;
    int spriteID = 0;

    if (!(*game_state)) {
        Sprite productionPage;
        LoadAndCacheImg((unsigned char *)"production.png", 4, &productionPage);

        float op = 0.0f;

        while(op < 1.0f) {
            RenderSingleSprite(productionPage, 0.0f, 0.0f, op);

            op += 0.05f;

            Sleep(10);
        }

        // Load Cache Images for this Scene
        LoadAndCacheImg((unsigned char *)"title.png", 4, &CacheArr[0]);
        LoadAndCacheImg((unsigned char *)"gameStartBtn.png", 4, &CacheArr[1]);
        LoadAndCacheImg((unsigned char *)"gameLoadBtn.png", 4, &CacheArr[2]);

        // Set Components in the Scene
        // Component no.0 = start button
        spriteID = 1;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], spriteID, {530.0f, 440.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        AddMouseEvent(&ComponentsArr[0], SceneButtonEvents, (void*)NovelScene, 0);

        // Component no.1 = load button
        spriteID = 2;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], spriteID, {530.0f, 560.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        AddMouseEvent(&ComponentsArr[1], SceneButtonEvents, (void*)NovelScene, 0);

        while(op > 0.0f) {
            RenderSingleSprite(productionPage, 0.0f, 0.0f, op);

            op -= 0.05f;

            Sleep(10);
        }
        
        productionPage.Release();
        
        (*game_state)++;
    }

    spriteID = 0;
    size = CacheArr[spriteID].ImgCache->GetPixelSize();
    SetRenderData(&RenderBuff[0], spriteID, {0.0f, 0.0f, (float)size.width, (float)size.height}, 1.0f);

    MatchRenderData(&RenderBuff[1], &ComponentsArr[0]);
    MatchRenderData(&RenderBuff[2], &ComponentsArr[1]);

    RenderAllComponents(RenderBuff, CacheArr, 3);
}

void LoadingScene(SceneFunc nextScene, int *game_state, ComponentData* ComponentsArr, RenderData* RenderBuff, Sprite* CacheArr) {
    if (updateState == LOADING) {
        for (int i = 0; i < 3; i++) {
            RenderLoadingAnimation(D2D1::ColorF(D2D1::ColorF::Black), i);
            Sleep(50);
        }

        if (nextScene) {
            nextScene(game_state, ComponentsArr, RenderBuff, CacheArr);
        }

        SetEvent(g_UpdateEvent);
    }
}