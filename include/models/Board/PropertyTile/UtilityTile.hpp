#pragma once
#include "PropertyTile.hpp"
using namespace std;

class UtilityTile : public PropertyTile
{ 
public:
    UtilityTile(int idx, string code, string name, int price, 
        int morgageValue, vector<int> rentPrices);
    void land(Player &p) override;
};
