#include "structs.h"
#include "ObjectController.hpp"
#include "Events.hpp"

ComponentData *LPressedComponent = nullptr;

void SceneButtonEvents(ComponentData* data, void* scene) {
    float left = data->position.x;
    float right = data->position.x + data->position.width;
    float top = data->position.y;
    float bottom = data->position.y + data->position.height;

    SceneFunc targetScene = (SceneFunc)scene;

    if (LPressedComponent == nullptr) {

        if ((mouseX > left && mouseX < right)&&(mouseY > top && mouseY < bottom)) {

            data->opacity = 0.5f;

            if (mouseLButtonPressed) {
                LPressedComponent = data;

                data->opacity = 0.0f;
            }

        }

        else data->opacity = 1.0f;
    }

    if (LPressedComponent == data) {

        if ((mouseX > left && mouseX < right)&&(mouseY > top && mouseY < bottom)) {
            data->opacity = 0.0f;

            if (!mouseLButtonPressed) {
                LPressedComponent = nullptr;

                nextScene = targetScene;
                updateState = LOADING;
                ResetEvent(g_UpdateEvent);
            }
        }
        else {
            data->opacity = 1.0f;

            if (!mouseLButtonPressed)
                LPressedComponent = nullptr;
        }
    }
}

void CharacterMoveEvent(ComponentData* data) {
    
}