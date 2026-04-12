#include "../../include/views/Button.hpp"

Button::Button(const std::string& text, Color bgColor, Color textColor) : text(text), bgColor(bgColor), textColor(textColor) {
    preferredHeight = 40;
    preferredWidth = MeasureText(text.c_str(), 20);
    disabled = false;
}

void Button::draw() {
    Color currentBg = disabled ? GRAY : bgColor;
    Color currentText = disabled ? DARKGRAY : textColor;
    DrawRectangle(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height, currentBg);
    DrawText(text.c_str(), boundingBox.x + 10, boundingBox.y + 10, 20, currentText);
}

void Button::setOnClick(std::function<void()> callback) {
    onButtonClick = callback;
}
