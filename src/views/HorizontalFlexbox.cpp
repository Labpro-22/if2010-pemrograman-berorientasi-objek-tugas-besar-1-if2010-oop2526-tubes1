#include "../../include/views/HorizontalFlexbox.hpp"

HorizontalFlexbox::HorizontalFlexbox() {
    spacing = 10.0f;
    padding = 10.0f;
    alignment = Alignment::START;
}

void HorizontalFlexbox::layout() {
    float xOffset = boundingBox.x + padding;
    for (auto& child : children) {
        float yOffset = boundingBox.y + padding;
        if (alignment == Alignment::CENTER) {
            yOffset += (boundingBox.height - padding * 2 - child->preferredHeight) / 2;
        } else if (alignment == Alignment::END) {
            yOffset += boundingBox.height - padding * 2 - child->preferredHeight;
        }
        child->setBoundary({xOffset, yOffset, child->preferredWidth, child->preferredHeight});
        xOffset += child->preferredWidth + spacing;
    }
}