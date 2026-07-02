#include "structs.h"
#include "ObjectController.hpp"
#include "Events.hpp"

void SceneButtonEvents(ComponentData* data, void* scene) {
    float left = data->position.x;
    float right = data->position.x + data->position.width;
    float top = data->position.y;
    float bottom = data->position.y + data->position.height;

    SceneFunc targetScene = (SceneFunc)scene;

    if ((mouseX > left && mouseX < right)&&(mouseY > top && mouseY < bottom)) {
        data->opacity = 0.5f; 

        if (mouseLButtonPressed) {
            data->opacity = 0.0f;

            nextScene = targetScene;
            updateState = LOADING;
            ResetEvent(g_UpdateEvent);
        }
    }
    else data->opacity = 1.0f;
}