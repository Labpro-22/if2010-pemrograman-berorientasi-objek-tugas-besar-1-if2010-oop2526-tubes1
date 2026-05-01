#include "../../include/views/Scene.hpp"
Scene::Scene(SceneManager* sm, GameManager* gm, AccountManager* am)
    : sceneManager(sm), gameManager(gm), accountManager(am) {}
