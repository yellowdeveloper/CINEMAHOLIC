#include <stdio.h>

#include "structs.h"
#include "ImgManager.hpp"
#include "ObjectController.hpp"
#include "InputManager.hpp"
#include "player.hpp"

void playerMove(ComponentData* self, float deltaTime) {
    playerData* data = (playerData*)self->scriptData;

    KeyState isUpPressed = inputEV.keyStateArr[0x26];
    KeyState isLeftPressed = inputEV.keyStateArr[0x25];
    KeyState isDownPressed = inputEV.keyStateArr[0x28];
    KeyState isRightPressed = inputEV.keyStateArr[0x27];

    KeyState isWPressed = inputEV.keyStateArr[0x57];
    KeyState isAPressed = inputEV.keyStateArr[0x41];
    KeyState isSPressed = inputEV.keyStateArr[0x53];
    KeyState isDPressed = inputEV.keyStateArr[0x44];

    float speed = data->speed;

    if (isUpPressed == PRESSED || isWPressed) {
        self->position.y -= speed * deltaTime;
        if (self->position.y < 0) self->position.y = 0.0f;
    }

    if (isLeftPressed == PRESSED || isAPressed) {
        self->position.x -= speed * deltaTime;
        if (self->position.x < 0) self->position.x = 0.0f;
    }

    if (isDownPressed == PRESSED || isSPressed) {
        self->position.y += speed * deltaTime;
        if (self->position.y > SCREEN_HD_H) self->position.y = SCREEN_HD_H * 1.0f;
    }

    if (isRightPressed == PRESSED || isDPressed) {
        self->position.x += speed * deltaTime;
        if (self->position.x > SCREEN_HD_W) self->position.x = SCREEN_HD_W* 1.0f;
    }

}