#ifndef ObjCtrl_H
#define ObjCtrl_H

#include <cstddef>

extern UpdateState updateState;
extern HANDLE g_UpdateEvent;

extern double d_time_sync;
extern double fps;

#define MAX_COMPONENT_NUM 1024

void ClearComponents(ComponentData* components, size_t max_compo);
void SetComponentData(ComponentData* data, int resourceID, RenderType renderType, Position pos, float scale, float opacity);
void UpdateThread(ComponentData* components, RenderContext *rContext);

#endif
