#ifndef ObjCtrl_H
#define ObjCtrl_H

extern int mouseX;
extern int mouseY;

void ButtonHover(ComponentData* data);
void UpdateThread(ComponentData* components, size_t max_compo);

#endif