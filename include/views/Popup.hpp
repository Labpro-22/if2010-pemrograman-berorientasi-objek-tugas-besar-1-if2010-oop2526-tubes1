#ifndef POPUP_HPP
#define POPUP_HPP
#include "VerticalFlexbox.hpp"
#include "UIElement.hpp"
class Popup : public UIElement {
private:
    VerticalFlexbox container;

public:
    bool isActive;
    void draw() override;
    void update() override;
    void add(UIElement* element);
};

#endif