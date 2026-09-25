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

void SetComponentData(ComponentData* data, int resourceID, Position pos, float scale, float opacity) {
    data->enabled = true;

    data->resourceID = resourceID;
    data->position.x = pos.x;
    data->position.y = pos.y;
    data->position.width = pos.width;
    data->position.height = pos.height;
    data->opacity = opacity;
}

void UpdateThread(ComponentData* components, RenderContext *rContext, size_t max_compo) {
    while(updateState != EXIT) {
        int rCount = 0;

        WaitForSingleObject(g_UpdateEvent, INFINITE);

        for (size_t  i = 0; i < max_compo; i++) {
            if (components[i].enabled) {
                rCount++;

                if (components[i].UpdateFunc) {
                    components[i].UpdateFunc(&components[i], d_time_sync);
                }

                if (components[i].OnClickFunc) {
                    components[i].OnClickFunc(&components[i]);
                }
                
                rContext->lock.lock();

                rContext->updateBuffer[i].enabled  = true;
                rContext->updateBuffer[i].resourceID = components[i].resourceID;
                rContext->updateBuffer[i].position = components[i].position;
                rContext->updateBuffer[i].opacity  = components[i].opacity;

                rContext->lock.unlock();
            }
        }

        rContext->renderCount = rCount;
        Sleep(1);
    }
}
