#ifndef Structs_H
#define Structs_H

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

typedef void (*MouseEvent)(struct ComponentData*);
typedef void (*UpdateFunc2)(struct ComponentData*);
typedef void (*UpdateFunc3)(struct ComponentData*);

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

    MouseEvent mouseEvent;
    UpdateFunc2 update2;
    UpdateFunc3 update3;
};

#endif