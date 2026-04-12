#include "../../include/views/Label.hpp"

Label::Label(const std::string& text, Color color, int size) : text (text), color(color), size(size) {
    preferredHeight = size;
    preferredWidth = MeasureText(text.c_str(), size);
}

void Label::draw() {
    DrawText(text.c_str(), boundingBox.x, boundingBox.y, size, color);
}

