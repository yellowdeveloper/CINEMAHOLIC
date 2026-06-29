#include "structs.h"
#include "ObjectController.hpp"

void ButtonHover(ComponentData* data) {
    if (data->opacity > 0) {
        data->opacity -= 0.05f;
    }
    else {
        data->opacity += 0.05f;
    }
}