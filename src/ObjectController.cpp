#include "structs.h"
#include "ObjectController.hpp"

void ClearComponents(ComponentData* components, size_t max_compo) {
    for (size_t  i = 0; i < max_compo; i++) {
        if (components[i].enabled) {
            components[i].enabled = false;
        }
    }
}

void SetComponentData(ComponentData* data, int spriteID, Position pos, float scale, float opacity) {
    data->enabled = true;
    data->spriteID = spriteID;
    data->position.x = pos.x;
    data->position.y = pos.y;
    data->position.width = pos.width;
    data->position.height = pos.height;
    data->opacity = opacity;
}

void AddMouseEvent(ComponentData* data, void (*func)(ComponentData*), int idx) {
    if (idx < MAX_MOUSE_EVENT) {
        data->mouseEvents[idx] = func;
    }
}

void AddUpdateEvent(ComponentData* data, void (*func)(ComponentData*), int idx) {
    if (idx < MAX_UPDATE_EVENT) {
        data->updateEvents[idx] = func;
    }
}

void UpdateThread(ComponentData* components, size_t max_compo) {
    while(1) {
        WaitForSingleObject(g_UpdateEvent, INFINITE);
        
        for (size_t  i = 0; i < max_compo; i++) {
            if (components[i].enabled) {
                for (int j = 0; j < MAX_MOUSE_EVENT; j++) {
                    if (components[i].mouseEvents[j]) {
                        components[i].mouseEvents[j](&components[i]);
                    }
                }

                for (int j = 0; j < MAX_UPDATE_EVENT; j++) {
                    if (components[i].updateEvents[j]) {
                        components[i].updateEvents[j](&components[i]);
                    }
                }
            }
        }
        Sleep(1);
    }
}
