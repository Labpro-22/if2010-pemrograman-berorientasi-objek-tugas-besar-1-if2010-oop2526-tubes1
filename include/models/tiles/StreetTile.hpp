#pragma once
#include "PropertyTile.hpp"

class StreetTile: public PropertyTile{
private:
    ColorGroup colorGroup;
    int rentTable[6];
    int houseCost;
    int hotelCost;
    int buildingLevel = 0;
    int festivalMultiplier = 1;
    int festivalDuration = 0;
public:
    void onLanded(Player& player, GameContext& gameContext) override;
    int calculateRent(int diceTotal, const GameContext& gameContext) override;

    void build();
    int sellBuilding();
    void applyFestival();
    void tickFestival();
    
    int getBuildingLevel() const;
    ColorGroup getColorGroup() const;
    bool canBuildNext(ColorGroup colorGroup) const;
    int getSellValueToBank() const;
    
    string getDisplayLabel() const override;
};