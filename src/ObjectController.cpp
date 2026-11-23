#include "structs.h"
#include "ObjectController.hpp"

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

        rContext->lock.lock();
        for (size_t  i = 0; components[i].enabled; i++) {
            rCount++;

            if (components[i].UpdateFunc) {
                components[i].UpdateFunc(&components[i], d_time_sync);
            }

            if (components[i].OnClickFunc) {
                components[i].OnClickFunc(&components[i]);
            }

            // 대사 변화 체크 (textUpdateRequest)

            rContext->updateBuffer[i].enabled  = true;
            rContext->updateBuffer[i].renderType = components[i].renderType;
            rContext->updateBuffer[i].resourceID = components[i].resourceID;
            rContext->updateBuffer[i].position = components[i].position;
            rContext->updateBuffer[i].opacity  = components[i].opacity;

        }
        rContext->renderCount = rCount;
        rContext->lock.unlock();

        Sleep(1);
    }
}
