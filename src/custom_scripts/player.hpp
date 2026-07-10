#ifndef Player_H
#define Player_H

struct playerData {
    float speed;
};

void playerMove(ComponentData* self, float deltaTime);

#endif /* Player_H */