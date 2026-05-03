#include "../../include/views/SceneManager.hpp"
#include "../../include/views/MainMenuScene.hpp"
#include "../../include/views/InGameScene.hpp"

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

void SceneManager::initialize(GameManager* gm, AccountManager* am) {
    gameManager    = gm;
    accountManager = am;
    mainMenuScene = std::make_unique<MainMenuScene>(this, gm, am);
    inGameScene   = std::make_unique<InGameScene>(this, gm, am);
    currentScene  = mainMenuScene.get();
}

void SceneManager::setScene(SceneType type) {
    if (type == SceneType::MainMenu) currentScene = mainMenuScene.get();
    else                              currentScene = inGameScene.get();
    if (currentScene) currentScene->onEnter();
}

void SceneManager::update() { if (currentScene) currentScene->update(); }
void SceneManager::draw()   { if (currentScene) currentScene->draw(); }
