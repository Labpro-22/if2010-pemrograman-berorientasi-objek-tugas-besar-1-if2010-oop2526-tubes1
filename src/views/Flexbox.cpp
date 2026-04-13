#include "../../include/views/Flexbox.hpp"

void Flexbox::add(UIElement* element) {
    children.push_back(element);
}

void Flexbox::draw() {
    for (auto& child : children) {
        child->draw();
    }
}

void Flexbox::update() {
    for (auto& child : children) {
        child->update();
    }
}