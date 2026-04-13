#ifndef VERTICALFLEXBOX_HPP
#define VERTICALFLEXBOX_HPP
#include "Flexbox.hpp"
class VerticalFlexbox : public Flexbox {
public:
    VerticalFlexbox();
    void layout() override;
};

#endif