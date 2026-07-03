#ifndef Scenes_H
#define Scenes_H

int NovelScene(int *game_state, ComponentData* ComponentsArr, RenderData* RenderBuff, Sprite* CacheArr);
int LoadTitle(int *game_state, ComponentData* ComponentsArr, RenderData* RenderBuff, Sprite* CacheArr);
void LoadingScene(SceneFunc *scene, int *game_state, ComponentData* ComponentsArr, RenderData* RenderBuff, Sprite* CacheArr);

#endif