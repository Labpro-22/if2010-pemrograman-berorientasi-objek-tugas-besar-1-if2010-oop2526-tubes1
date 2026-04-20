#pragma once
#include "PropertyTile.hpp"

class Railroad : public PropertyTile
{
public:
    Railroad(int idx, string code, string name, int price, int morgageValue);
    PropertyStatus land(Player &p) override;
};
