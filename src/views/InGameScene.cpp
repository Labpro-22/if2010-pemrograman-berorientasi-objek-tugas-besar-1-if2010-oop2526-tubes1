#include "../../include/views/InGameScene.hpp"

void InGameScene::draw() {
    root.setBoundary(boundingBox);
    root.layout();
    root.draw();
    if (currentPopup) {
        currentPopup->draw();
    }
}

void InGameScene::update() {
    root.update();
    if (currentPopup) {
        currentPopup->update();
    }
}