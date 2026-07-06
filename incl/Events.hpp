#ifndef Events_H
#define Events_H

extern int mouseX;
extern int mouseY;

extern bool mouseLButtonPressed;
extern bool mouseRButtonPressed;

extern bool upButtonPressed;
extern bool leftButtonPressed;
extern bool downButtonPressed;
extern bool rightButtonPressed;

extern SceneFunc nextScene;

void SceneButtonEvents(ComponentData* data, void* scene);
void CharacterMoveEvent(ComponentData* data);

#endif /* Events_H */