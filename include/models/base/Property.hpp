#pragma once
#include <string>
#include "models/base/GameTypes.hpp"
#include "models/base/Tile.hpp"

class Player;

class Property : public Tile
{
protected:
    PropertyType propertyType;
    PropertyStatus status;
    Player *owner;
    int purchasePrice;
    int mortgageValue;
    int festivalMultiplierSteps;
    int festivalTurnsRemaining;

public:
    Property(const std::string &code,
             const std::string &name,
             int position,
             PropertyType propertyType,
             int purchasePrice,
             int mortgageValue);
    virtual ~Property() = default;

    PropertyType getPropertyType() const;
    PropertyStatus getStatus() const;
    Player *getOwner() const;
    int getPurchasePrice() const;
    int getMortgageValue() const;

    bool isOwned() const;
    bool isMortgaged() const;

    void setOwner(Player *newOwner);
    void clearOwner();
    void setStatus(PropertyStatus newStatus);
    void mortgage();
    void unmortgage();

    int getFestivalMultiplierSteps() const;
    int getFestivalTurnsRemaining() const;
    int getFestivalMultiplier() const;
    void applyFestivalBoost();
    void restoreFestivalState(int multiplierSteps, int turnsRemaining);
    void tickFestival();
    void clearFestival();

    void onLanded(GameManager &game, Player &player, int diceTotal) override;
    std::string getDisplayType() const override;

    virtual int getCurrentRent(int diceTotal) const = 0;
    virtual int getAssetValue() const = 0;
    virtual bool hasBuildings() const = 0;
};
