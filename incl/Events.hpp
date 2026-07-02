#ifndef Events_H
#define Events_H

extern int mouseX;
extern int mouseY;

extern bool mouseLButtonPressed;
extern bool mouseRButtonPressed;

extern SceneFunc nextScene;

void SceneButtonEvents(ComponentData* data, void* scene);

#endif /* Events_H */