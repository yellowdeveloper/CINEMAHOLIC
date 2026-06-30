#ifndef ObjCtrl_H
#define ObjCtrl_H

extern UpdateState updateState;
extern HANDLE g_UpdateEvent;

void ClearComponents(ComponentData* components, size_t max_compo);
void SetComponentData(ComponentData* data, int spriteID, Position pos, float scale, float opacity);
void AddMouseEvent(ComponentData* data, void (*func)(ComponentData*), int idx);
void AddUpdateEvent(ComponentData* data, void (*func)(ComponentData*), int idx);
void UpdateThread(ComponentData* components, size_t max_compo);

#endif