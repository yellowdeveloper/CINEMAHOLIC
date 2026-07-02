#ifndef Scenes_H
#define Scenes_H

void NovelScene(int *game_state, ComponentData* ComponentsArr, RenderData* RenderBuff, Sprite* CacheArr);
void LoadTitle(int *game_state, ComponentData* ComponentsArr, RenderData* RenderBuff, Sprite* CacheArr);
void LoadingScene(SceneFunc nextScene, int *game_state, ComponentData* ComponentsArr, RenderData* RenderBuff, Sprite* CacheArr);

#endif