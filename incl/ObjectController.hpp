#ifndef ObjCtrl_H
#define ObjCtrl_H

#include <cstddef>
#include <condition_variable>
#include <mutex>

extern UpdateState updateState;
extern HANDLE g_UpdateEvent;

extern double d_time_sync;
extern double fps;

extern std::mutex tickMutex;
extern std::condition_variable tickCv;

extern unsigned pendingTicks;
extern bool stopping;

#define MAX_COMPONENT_NUM 1024

#define UPDATE_HZ 180
#define UPDATE_TIME float(1000.0f / UPDATE_HZ)

void ClearComponents(ComponentData* components, size_t max_compo);
void SetComponentData(ComponentData* data, int resourceID, RenderType renderType, Position pos, float scale, float opacity);
void UpdateThread(ComponentData* components, RenderContext *rContext);

#endif
