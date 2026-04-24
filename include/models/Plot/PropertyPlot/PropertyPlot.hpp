#pragma once

#include "models/Plot/Plot.hpp"
#include "models/Plot/PropertyPlot/PropertyStatus.hpp"

class PropertyPlot: public Plot{
protected:
    int buyPrice;
    int mortgageValue;
    PropertyStatus propertyStatus;
    Player* owner;
    int festivalDuration;
    int festivalMultiplier;

    bool isFestival() const;

public:
    PropertyPlot(std::string name, std::string code, Color color, int buyPrice, int mortgageValue,
                 Player* owner, PropertyStatus propertyStatus, int festivalDuration, int festivalMultiplier);
    virtual ~PropertyPlot() = default;

    int getBuyPrice() const;
    int getMortgageValue() const;
    PropertyStatus getPropertyStatus() const;
    int getFestivalDuration() const;
    int getFestivalMultiplier() const;
    virtual std::map<int, int> getRentPriceTable() const = 0;
    virtual int getRentPrice(int level) const = 0;
    Player* getOwner() const override;
    void setOwner(Player* player);

    bool isOwned() const;
    bool isMortgaged() const;
    void applyFestival();
    void endFestival();
    virtual int calculateRentPrice(PlotContext& ctx) const = 0;
};