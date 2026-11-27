#include "structs.h"
#include "ObjectController.hpp"

#ifndef QPC
#define QPC 0
#endif

#if QPC
#include "QPC_MemBuf.hpp"
#endif

void ClearComponents(ComponentData* components, size_t max_compo) {
    for (size_t  i = 0; i < max_compo; i++) {
        if (components[i].enabled) {
            components[i].enabled = false;

            components[i].UpdateFunc = nullptr;
            components[i].OnClickFunc = nullptr;
        }
    }
}

void SetComponentData(ComponentData* data, int resourceID, RenderType renderType, Position pos, float scale, float opacity) {
    data->enabled = true;

    data->resourceID = resourceID;
    data->renderType = renderType;
    data->position.x = pos.x;
    data->position.y = pos.y;
    data->position.width = pos.width;
    data->position.height = pos.height;
    data->opacity = opacity;
}

void UpdateThread(ComponentData* components, RenderContext *rContext) {
    while(updateState != EXIT) {
        int rCount = 0;

        WaitForSingleObject(g_UpdateEvent, INFINITE);

        std::unique_lock<std::mutex> lock(tickMutex);
        tickCv.wait(lock, [] { return stopping || pendingTicks > 0; });

        if (stopping) break;
        --pendingTicks;
        lock.unlock();

        const float updateDeltaTime = UPDATE_TIME;

#if QPC
        Record(QPC_UPDATE_LOCK_REQUEST);
#endif
        rContext->lock.lock();
#if QPC
        Record(QPC_UPDATE_LOCK_ACQUIRED);
#endif
        for (size_t  i = 0; components[i].enabled && i < MAX_COMPONENT_NUM; i++) {
            rCount++;

            if (components[i].UpdateFunc) {
#if QPC
                Record(QPC_UPDATE_FUNC_START, updateDeltaTime);
                components[i].UpdateFunc(&components[i], updateDeltaTime);
                Record(QPC_UPDATE_FUNC_END, updateDeltaTime);
#else
                components[i].UpdateFunc(&components[i], updateDeltaTime);
#endif
            }

            if (components[i].OnClickFunc) {
                components[i].OnClickFunc(&components[i]);
            }

            // TODO::대사 변화 체크 (textUpdateRequest)

            rContext->updateBuffer[i].enabled  = true;
            rContext->updateBuffer[i].renderType = components[i].renderType;
            rContext->updateBuffer[i].resourceID = components[i].resourceID;
            rContext->updateBuffer[i].position = components[i].position;
            rContext->updateBuffer[i].opacity  = components[i].opacity;

        }
        rContext->renderCount = rCount;
        rContext->lock.unlock();
#if QPC
        Record(QPC_UPDATE_LOCK_RELEASED);

        Record(QPC_SLEEP_START);
#endif
        // Sleep(1);
#if QPC
        Record(QPC_SLEEP_END);
#endif
    }
}
