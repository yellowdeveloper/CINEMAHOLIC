#include <stdio.h>

#include "structs.h"
#include "ObjectController.hpp"
#include "InputManager.hpp"
#include "sceneChangeButtonEvent.hpp"

ComponentData *LPressedComponent = nullptr;

void SceneButtonEvents(ComponentData* self) {
    float left = self->position.x;
    float right = self->position.x + self->position.width;
    float top = self->position.y;
    float bottom = self->position.y + self->position.height;

    sceneChangeButtonData* data = (sceneChangeButtonData*)self->scriptData;

    SceneFunc targetScene = (SceneFunc)data->scene;

    if (LPressedComponent == nullptr) {

        if ((mouseX > left && mouseX < right)&&(mouseY > top && mouseY < bottom)) {

            self->opacity = 0.5f;

            if (mouseLButtonPressed) {
                LPressedComponent = self;

                self->opacity = 0.0f;
            }

        }

        else self->opacity = 1.0f;
    }

    if (LPressedComponent == self) {

        if ((mouseX > left && mouseX < right)&&(mouseY > top && mouseY < bottom)) {
            self->opacity = 0.0f;

            if (!mouseLButtonPressed) {
                LPressedComponent = nullptr;

                nextScene = targetScene;
                updateState = LOADING;
                ResetEvent(g_UpdateEvent);
            }
        }
        else {
            self->opacity = 1.0f;

            if (!mouseLButtonPressed)
                LPressedComponent = nullptr;
        }
    }
}