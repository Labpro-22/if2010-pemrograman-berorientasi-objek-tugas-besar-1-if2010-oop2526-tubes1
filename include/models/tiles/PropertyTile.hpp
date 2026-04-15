#pragma once
#include "Tile.hpp"

class PropertyTile: public Tile{
private:
    Player* owner = nullptr;
    PropertyStatus status;
    int buyPrice;
    int mortgageValue;
public:
    PropertyTile();
    PropertyTile(Player& owner, PropertyStatus status, int buyPrice, int mortgageValue, int index, string code, string name, TileCategory tileCategory);
    virtual ~PropertyTile() = default;

    virtual int calculateRent(int diceTotal, const GameContext& gameContext) = 0;

    void mortgage();
    void redeem();
    void transferTo(Player& newOwner);
    void returnToBank();

    bool isOwnedBy(Player& player) const;
    bool isMortgaged() const;
    Player* getOwner() const;
    PropertyStatus getStatus() const;
    int getBuyPrice() const;
    int getMortgageValue() const;
    virtual int getSellValueToBank() const = 0;
    
};
