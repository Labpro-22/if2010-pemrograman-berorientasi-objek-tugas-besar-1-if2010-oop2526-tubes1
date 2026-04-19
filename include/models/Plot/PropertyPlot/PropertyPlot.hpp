#pragma once

#include "models/Plot/Plot.hpp"
#include "models/Plot/PropertyPlot/PropertyStatus.hpp"
#include "models/Player/Player.hpp"

class PropertyPlot: public Plot{
protected:
    int mortgageValue;
    PropertyStatus propertyStatus;
    // Player* ownedBy;
    int festivalDuration;
    int festivalMultiplier; //TODO: color disimpan di property, bukan di land

    bool isFestival() const;

public:
    PropertyPlot(std::string name, std::string code, int mortgageValue, PropertyStatus propertyStatus);
    virtual ~PropertyPlot() = default;

    int getMortgageValue() const;
    PropertyStatus getPropertyStatus() const;
    int getFestivalDuration() const;
    int getFestivalMultiplier() const;
    virtual std::map<int, int> getRentPriceTable() const = 0;
    virtual int getRentPrice(int level) const = 0;

    bool isMortgaged() const;
    void applyFestival();
    void endFestival();
    virtual int calculateRentPrice() const = 0;
};