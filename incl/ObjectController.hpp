#ifndef ObjCtrl_H
#define ObjCtrl_H

extern UpdateState updateState;
extern HANDLE g_UpdateEvent;

void ClearComponents(ComponentData* components, size_t max_compo);
void SetComponentData(ComponentData* data, int spriteID, Position pos, float scale, float opacity);
void UpdateThread(ComponentData* components, RenderContext *rContext, size_t max_compo);

#endif