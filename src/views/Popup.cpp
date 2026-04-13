#include "../../include/views/Popup.hpp"

void Popup::draw() {
    if (isActive) {
        DrawRectangle(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height, {0, 0, 0, 200});
        container.setBoundary(boundingBox);
        container.layout();
        container.draw();
    }
}

void Popup::update() {
    if (isActive) {
        container.update();
    }
}

void Popup::add(UIElement* element) {
    container.add(element);
}