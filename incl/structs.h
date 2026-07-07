#ifndef CommonStructs_H
#define CommonStructs_H

#include <windows.h>
#include "d2d1_1.h"
#include <mutex>

/// @brief 이미지 크기 구조체
struct ImgSize {
    int imgHeight;
    int imgWidth;
    int imgChannels;
};

/**
 * @brief 렌더링 위치 구조체
 * @note width와 height는 주로 D2D_SIZE_U 구조체에서 가져와 할당한다.
 */
struct Position
{
    float x;
    float y;
    float width;
    float height;
};

/// @brief 이미지 캐시 구조체
struct Sprite {
    ///< 비트맵 캐시
    ID2D1Bitmap* ImgCache = nullptr; 

    ///< 할당된 비트맵 자원을 해제 및 포인터를 초기화
    void Release() {
        ImgCache->Release();
        ImgCache = nullptr;
    } 
};

/// @brief 렌더링 정보 구조체
/// @note 멤버 변수들의 특성은 컴포넌트 데이터 참고
struct RenderData {
    int spriteID;
    Position position;
    float opacity;
    
    bool enabled;
};

#define MAX_MOUSE_EVENT 5
#define MAX_UPDATE_EVENT 5

/// @brief 컴포넌트 정보 구조체
struct ComponentData {
    ///< 컴포넌트의 활성화 여부
    bool enabled;

    // 정말 필요한가?
    int componentType;

    ///< 캐싱된 이미지 배열과 매핑할 ID (캐시 배열의 인덱스)
    int spriteID;

    ///< 렌더 스냅샷(RenderData)에 업데이트 할 위치 정보
    Position position;
    ///< 렌더 스냅샷(RenderData)에 업데이트 할 투명도 정보
    float opacity;
    //< scale 구현이 필요한가?
    float scale;


    void* scriptData = nullptr;

    void (*UpdateFunc)(ComponentData* self, float deltaTime) = nullptr;
    void (*OnClickFunc)(ComponentData* self) = nullptr;
};

/**
 * @brief 업데이트 상태 열거형
 * @note 레이스 컨디션 방지 및 스레드 동기화를 위해 사용
 */
typedef enum UpdateState {
    LOADING,     ///< 로딩 중 상태
    PROCESSING,  ///< 게임 진행 중 상태
    PAUSED,      ///< 정지 상태
    EXIT         ///< 종료 상태
};

/**
 * @brief 렌더 버퍼 관리용
 * @note 레이스 컨디션 방지 및 스레드 동기화를 위해 사용
 */
typedef struct RenderContext {
    RenderData* updateBuffer = nullptr;
    RenderData* renderBuffer = nullptr;

    int renderCount;
    std::mutex lock;
};

typedef void (*SceneFunc)(int*, ComponentData*, Sprite*);

#endif