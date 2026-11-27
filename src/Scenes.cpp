#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>

#include "structs.h"
#include "ImgManager.hpp"
#include "ObjectController.hpp"
#include "InputManager.hpp"
#include "Scenes.hpp"
#include "TextManager.hpp"

// custom_scripts to add events
#include "custom_scripts/sceneChangeButtonEvent.hpp"
#include "custom_scripts/player.hpp"

void MiniGameScene(GameState *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
    int resourceID = 0;
    D2D1_SIZE_U size;

    if (updateState == LOADING) {
        JsonScriptParser((unsigned char *)"res/scripts/Scene1.json");

        CacheArr[0].ImgCache = GetSolidSprite(0xFFB0C9FF);
        int Idx = 1;
        Idx = CacheAllImgsFromBuffer(4, Idx, CacheArr);
        CacheArr[Idx].ImgCache = GetRenderedText(L"Hello World", L"Arial", 24.0f, D2D1::ColorF::Black);

        resourceID = 0;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], resourceID, RenderType::Sprite, {0.0f, 0.0f, SCREEN_HD_W, SCREEN_HD_H}, 1.0f, 1.0f);

        resourceID = 1;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], resourceID, RenderType::Sprite, {0.0f, 0.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);

        resourceID = 2;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[2], resourceID, RenderType::Sprite, {0.0f, 0.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);

        resourceID = 3;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[3], resourceID, RenderType::Text, {605.0f, 286.5f, (float)size.width, (float)size.height}, 1.0f, 1.0f);

        *game_state = MINIGAME;

        updateState = PROCESSING;
    }
}

void TycoonScene(GameState *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
    int resourceID = 0;
    D2D1_SIZE_U size;

    if (updateState == LOADING) {
        CacheArr[0].ImgCache = GetSolidSprite(0xFFF58742);
        LoadAndCacheImg((unsigned char *)"dotMainCrStand.png", 4, &CacheArr[1]);
        // LoadAndCacheImg((unsigned char *)".png", 4, &CacheArr[2]);

        resourceID = 0;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], resourceID, RenderType::Sprite, {0.0f, 0.0f, SCREEN_HD_W, SCREEN_HD_H}, 1.0f, 1.0f);

        resourceID = 1;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], resourceID, RenderType::Sprite, {605.0f, 286.5f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[1].scriptData = new playerData{
            1.0f
        };
        ComponentsArr[1].UpdateFunc = playerMove;

        *game_state = TYCOON;

        updateState = PROCESSING;
    }
}

void NovelScene(GameState *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
    int resourceID = 0;
    D2D1_SIZE_U size;

    if (updateState == LOADING) {
        CacheArr[0].ImgCache = GetSolidSprite(0xFFB0C9FF);
        LoadAndCacheImg((unsigned char *)"character1_standing.png", 4, &CacheArr[1]);
        LoadAndCacheImg((unsigned char *)"bottom_panel.png", 4, &CacheArr[2]);

        resourceID = 0;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], resourceID, RenderType::Sprite, {0.0f, 0.0f, SCREEN_HD_W, SCREEN_HD_H}, 1.0f, 1.0f);
        
        resourceID = 1;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], resourceID, RenderType::Sprite, {0.0f, 223.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);

        resourceID = 2;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[2], resourceID, RenderType::Sprite, {150.0f, 375.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);

        *game_state = NOVEL;

        updateState = PROCESSING;
    }
}

void TestScene(GameState *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
    int resourceID = 0;
    D2D1_SIZE_U size;

    if (updateState == LOADING) {
        LoadAndCacheImg((unsigned char *)"title.png", 4, &CacheArr[0]);
        CacheArr[1].ImgCache = GetRenderedText(L"Novel", L"Arial", 24.0f, D2D1::ColorF::Black);
        CacheArr[2].ImgCache = GetRenderedText(L"Tycoon", L"Arial", 24.0f, D2D1::ColorF::Black);
        CacheArr[3].ImgCache = GetRenderedText(L"Mini Game", L"Arial", 24.0f, D2D1::ColorF::Black);

        resourceID = 0;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], resourceID, RenderType::Sprite, {0.0f, 0.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        
        resourceID = 1;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], resourceID, RenderType::Text, {586.0f, 440.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[1].scriptData = new sceneChangeButtonData{
            NovelScene
        };
        ComponentsArr[1].OnClickFunc = SceneButtonEvents;

        resourceID = 2;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[2], resourceID, RenderType::Text, {576.0f, 500.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[2].scriptData = new sceneChangeButtonData{
            TycoonScene
        };
        ComponentsArr[2].OnClickFunc = SceneButtonEvents;

        resourceID = 3;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[3], resourceID, RenderType::Text, {543.0f, 560.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[3].scriptData = new sceneChangeButtonData{
            MiniGameScene
        };
        ComponentsArr[3].OnClickFunc = SceneButtonEvents;

        *game_state = NOVEL;

        updateState = PROCESSING;        
    }
}

// Title Screen : Scene number 0 ~ 1
void LoadTitle(GameState *game_state, ComponentData* ComponentsArr, Sprite* CacheArr) {
    D2D1_SIZE_U size;
    int resourceID = 0;

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
        CacheArr[1].ImgCache = GetRenderedText(L"Start", L"Arial", 24.0f, D2D1::ColorF::Black);
        CacheArr[2].ImgCache = GetRenderedText(L"Load", L"Arial", 24.0f, D2D1::ColorF::Black);

        // Set Components in the Scene
        resourceID = 0;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[0], resourceID, RenderType::Sprite, {0.0f, 0.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);

        resourceID = 1;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[1], resourceID, RenderType::Text, {530.0f, 440.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
        ComponentsArr[1].scriptData = new sceneChangeButtonData{
            TestScene
        };
        ComponentsArr[1].OnClickFunc = SceneButtonEvents;

        resourceID = 2;
        size = CacheArr[resourceID].ImgCache->GetPixelSize();
        SetComponentData(&ComponentsArr[2], resourceID, RenderType::Text, {530.0f, 560.0f, (float)size.width, (float)size.height}, 1.0f, 1.0f);
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
        
        *game_state = TITLE;
    }
}

void LoadingScene(SceneFunc *scene, GameState *game_state, ComponentData *ComponentsArr, Sprite *CacheArr) {
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