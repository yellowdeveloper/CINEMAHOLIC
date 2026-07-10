#include <windows.h>
#include <d2d1.h>

#include "structs.h"
#include "ImgManager.hpp"
#include "ObjectController.hpp"
#include "InputManager.hpp"
#include "Scenes.hpp"

// custom_scripts to add events
#include "custom_scripts/sceneChangeButtonEvent.hpp"
#include "custom_scripts/player.hpp"

void MiniGameScene(int *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
    if (updateState == LOADING) {

        *game_state = 4;

        updateState = PROCESSING;
    }
}

void TycoonScene(int *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
    int spriteID = 0;
    D2D1_SIZE_U size;

    if (updateState == LOADING) {
        CacheArr[0].ImgCache = GetSolidSprite(0xFFF58742);
        LoadAndCacheImg((unsigned char *)"dotMainCrStand.png", 4, &CacheArr[1]);
        // LoadAndCacheImg((unsigned char *)".png", 4, &CacheArr[2]);

        spriteID = 0;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], spriteID, {0.0f, 0.0f, SCREEN_HD_W, SCREEN_HD_H}, 1.0f, 1.0f);

        spriteID = 1;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], spriteID, {605.0f, 286.5f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[1].scriptData = new playerData{
            0.05f
        };
        ComponentsArr[1].UpdateFunc = playerMove;

        *game_state = 2;

        updateState = PROCESSING;
    }
}

void NovelScene(int *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
    int spriteID = 0;
    D2D1_SIZE_U size;

    if (updateState == LOADING) {
        CacheArr[0].ImgCache = GetSolidSprite(0xFFB0C9FF);
        LoadAndCacheImg((unsigned char *)"character1_standing.png", 4, &CacheArr[1]);
        LoadAndCacheImg((unsigned char *)"bottom_panel.png", 4, &CacheArr[2]);

        spriteID = 0;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], spriteID, {0.0f, 0.0f, SCREEN_HD_W, SCREEN_HD_H}, 1.0f, 1.0f);
        
        spriteID = 1;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], spriteID, {0.0f, 223.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);

        spriteID = 2;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[2], spriteID, {150.0f, 375.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);

        *game_state = 3;

        updateState = PROCESSING;
    }
}

void TestScene(int *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
    int spriteID = 0;
    D2D1_SIZE_U size;

    if (updateState == LOADING) {    
        LoadAndCacheImg((unsigned char *)"title.png", 4, &CacheArr[0]);
        LoadAndCacheImg((unsigned char *)"novelBtn.png", 4, &CacheArr[1]);
        LoadAndCacheImg((unsigned char *)"tycoonBtn.png", 4, &CacheArr[2]);
        LoadAndCacheImg((unsigned char *)"miniGameBtn.png", 4, &CacheArr[3]);

        spriteID = 0;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], spriteID, {0.0f, 0.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        
        spriteID = 1;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], spriteID, {586.0f, 440.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[1].scriptData = new sceneChangeButtonData{
            NovelScene
        };
        ComponentsArr[1].OnClickFunc = SceneButtonEvents;

        spriteID = 2;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[2], spriteID, {576.0f, 500.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[2].scriptData = new sceneChangeButtonData{
            TycoonScene
        };
        ComponentsArr[2].OnClickFunc = SceneButtonEvents;

        spriteID = 3;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[3], spriteID, {543.0f, 560.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[3].scriptData = new sceneChangeButtonData{
            MiniGameScene
        };
        ComponentsArr[3].OnClickFunc = SceneButtonEvents;

        *game_state = 3;

        updateState = PROCESSING;        
    }
}

// Title Screen : Scene number 0 ~ 1
void LoadTitle(int *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
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
        spriteID = 0;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], spriteID, {0.0f, 0.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);

        spriteID = 1;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], spriteID, {530.0f, 440.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[1].scriptData = new sceneChangeButtonData{
            TestScene
        };
        ComponentsArr[1].OnClickFunc = SceneButtonEvents;

        spriteID = 2;
        size = CacheArr[spriteID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[2], spriteID, {530.0f, 560.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[2].scriptData = new sceneChangeButtonData{
            TestScene
        };
        ComponentsArr[2].OnClickFunc = SceneButtonEvents;

        while(op > 0.0f) {
            RenderSingleSprite(productionPage, 0.0f, 0.0f, op);

            op -= 0.05f;

            Sleep(10);
        }
        
        productionPage.Release();
        
        *game_state = 1;
    }
}

void LoadingScene(SceneFunc *scene, int *game_state, ComponentData *ComponentsArr, Sprite *CacheArr) {
    if (updateState == LOADING) {
        for (int i = 0; i < 3; i++) {
            RenderLoadingAnimation(D2D1::ColorF(D2D1::ColorF::Black), i);
            Sleep(50);
        }

        if (nextScene) {
            nextScene(game_state, ComponentsArr, CacheArr);
            *scene = nextScene;
        }

        SetEvent(g_UpdateEvent);
    }
}