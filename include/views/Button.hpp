#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <string>
#include <functional>
#include "UIElement.hpp"

class Button : public UIElement {
public:
    Button(const std::string& text, Color bgColor, Color textColor);
    void draw() override;
    bool disabled;
    void setOnClick(std::function<void()> callback);

private:
    std::string text;
    Color bgColor;
    Color textColor;
    std::function<void()> onButtonClick;
};

#endif