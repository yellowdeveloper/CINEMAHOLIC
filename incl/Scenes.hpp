#ifndef Scenes_H
#define Scenes_H

extern SceneFunc nextScene;

void NovelScene(int *game_state, ComponentData* ComponentsArr, Sprite* CacheArr);
void TestScene(int *game_state, ComponentData* ComponentsArr, Sprite* CacheArr);
void LoadTitle(int *game_state, ComponentData* ComponentsArr, Sprite* CacheArr);
void LoadingScene(SceneFunc *scene, int *game_state, ComponentData* ComponentsArr, Sprite* CacheArr);

#endif