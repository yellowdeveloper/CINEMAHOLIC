#include "structs.h"
#include "ObjectController.hpp"

void ButtonHover(ComponentData* data) {
    float left = data->position.x;
    float right = data->position.x + data->position.width;
    float top = data->position.y;
    float bottom = data->position.y + data->position.height;

    if ((mouseX > left && mouseX < right)&&(mouseY > top && mouseY < bottom)) {
        data->opacity = 0.5f; 
    }
    else data->opacity = 1.0f; 
}

void UpdateThread(ComponentData* components, size_t max_compo) {
    while(1) {
        for (size_t  i = 0; i < max_compo; i++) {
            if (components[i].enabled) {
                if (components[i].mouseEvent)
                    components[i].mouseEvent(&components[i]);

                if (components[i].update2)
                    components[i].update2(&components[i]);

                if (components[i].update3)
                    components[i].update3(&components[i]);
            }
        }
    }
}
