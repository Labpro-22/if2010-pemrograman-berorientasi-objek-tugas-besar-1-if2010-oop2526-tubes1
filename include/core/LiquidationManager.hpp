#pragma once

#include <vector>

using namespace std;

class Player;
class PropertyTile;

class LiquidationManager {
private:
    int calculatePrice(const Player& player, const PropertyTile* property) const;

public:
    LiquidationManager() = default;
    ~LiquidationManager() = default;

    void liquidateAssets(Player& player, const vector<PropertyTile*>& targetProp);
};