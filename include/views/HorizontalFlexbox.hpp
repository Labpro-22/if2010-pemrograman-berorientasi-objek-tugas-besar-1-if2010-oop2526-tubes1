#ifndef HORIZONTALFLEXBOX_HPP
#define HORIZONTALFLEXBOX_HPP
#include "Flexbox.hpp"
class HorizontalFlexbox : public Flexbox {
public:
    HorizontalFlexbox();
    void layout() override;
};

#endif