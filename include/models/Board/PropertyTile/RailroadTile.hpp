#pragma once
#include "PropertyTile.hpp"

class RailroadTile : public PropertyTile
{
public:
    RailroadTile(int idx, string code, string name, int price, int morgageValue, vector<int> rentPrices);
    void land(Player &p) override;
};
