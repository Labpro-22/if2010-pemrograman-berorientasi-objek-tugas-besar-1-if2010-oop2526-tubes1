#include "../../include/views/Scene.hpp"

void Scene::showPopup(const std::string& key) {
    auto it = popups.find(key);
    if (it != popups.end()) {
        currentPopup = &it->second;
    }
}

void Scene::collapsePopup() {
    currentPopup = nullptr;
}