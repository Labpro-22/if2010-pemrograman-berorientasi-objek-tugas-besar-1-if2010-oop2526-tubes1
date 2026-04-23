#pragma once

#include "models/Plot/Plot.hpp"
#include "models/Plot/PropertyPlot/PropertyStatus.hpp"

class PropertyPlot: public Plot{
protected:
    Color color;
    int mortgageValue;
    PropertyStatus propertyStatus;
    Player* owner;
    int festivalDuration;
    int festivalMultiplier;

    bool isFestival() const;

public:
    PropertyPlot(std::string name, std::string code, Color color, int mortgageValue, PropertyStatus propertyStatus);
    virtual ~PropertyPlot() = default;

    int getMortgageValue() const;
    PropertyStatus getPropertyStatus() const;
    int getFestivalDuration() const;
    int getFestivalMultiplier() const;
    virtual std::map<int, int> getRentPriceTable() const = 0;
    virtual int getRentPrice(int level) const = 0;
    Color getColor() const override;
    Player* getOwner() const override;

    bool isMortgaged() const;
    void applyFestival();
    void endFestival();
    virtual int calculateRentPrice() const = 0;
};