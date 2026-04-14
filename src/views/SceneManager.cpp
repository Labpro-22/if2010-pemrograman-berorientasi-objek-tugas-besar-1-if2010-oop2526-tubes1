#include "../../include/views/SceneManager.hpp"

void SceneManager::setScene(Scene* newScene) {
    currentScene = newScene;
}

void SceneManager::update() {
    if (currentScene != nullptr) {
        currentScene->update();
    }
}

void SceneManager::draw() {
    if (currentScene != nullptr) {
        currentScene->draw();
    }
}