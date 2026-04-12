#ifndef LABEL_HPP
#define LABEL_HPP
#include <string>
#include "UIElement.hpp"

class Label : public UIElement {
public:
    Label(const std::string& text, Color color, int size);
    void draw() override;

private:
    std::string text;
    Color color;
    int size;
};

#endif