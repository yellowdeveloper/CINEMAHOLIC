#ifndef Structs_H
#define Structs_H

#include <windows.h>
#include "d2d1_1.h"

struct ImgSize {
    int imgHeight;
    int imgWidth;
    int imgChannels;
};

struct Position
{
    float x;
    float y;
    float width;
    float height;
};

struct Sprite {
    ID2D1Bitmap* ImgCache;
    void Release() {
        ImgCache->Release();
        ImgCache = nullptr;
    }
};

struct RenderData {
    int spriteID;
    Position position;
    float opacity;
    
    bool enabled;
};

#define MAX_MOUSE_EVENT 5
#define MAX_UPDATE_EVENT 5

typedef void (*MouseEvent)(struct ComponentData*, void* customArgs1);
typedef void (*UpdateFunc)(struct ComponentData*);

struct ComponentData {
    // 정말 필요한가?
    int componentType;

    // 
    int spriteID;

    // 렌더 스냅샷을 생성할 렌더링 정보
    Position position;
    float opacity;
    // scale 구현이 필요한가?
    float scale;

    bool enabled;

    MouseEvent mouseEvents[MAX_MOUSE_EVENT];

    void* mouseEvCustomArgs[MAX_MOUSE_EVENT];

    UpdateFunc updateEvents[MAX_UPDATE_EVENT];
};

typedef enum UpdateState {
    LOADING,
    PROCESSING,
    PAUSED,
    EXIT
};

typedef void (*SceneFunc)(int*, ComponentData*, RenderData*, Sprite*);

#endif