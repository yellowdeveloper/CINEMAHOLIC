#ifndef Scenes_H
#define Scenes_H

extern SceneFunc nextScene;

void NovelScene(GameState *game_state, ComponentData* ComponentsArr, Sprite* CacheArr);
void TestScene(GameState *game_state, ComponentData* ComponentsArr, Sprite* CacheArr);
void LoadTitle(GameState *game_state, ComponentData* ComponentsArr, Sprite* CacheArr);
void LoadingScene(SceneFunc *scene, GameState *game_state, ComponentData* ComponentsArr, Sprite* CacheArr);

#endif