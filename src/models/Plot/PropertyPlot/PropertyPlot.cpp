#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

    PropertyPlot::PropertyPlot(std::string name, std::string code, Color color, int mortgageValue, PropertyStatus propertyStatus = PropertyStatus::BANK):
        Plot(name, code),
        color(color),
        mortgageValue(mortgageValue),
        propertyStatus(propertyStatus) //TODO add ownedBy and festivalDuration
        {}

    int PropertyPlot::getMortgageValue() const{
        return mortgageValue;
    }

    PropertyStatus PropertyPlot::getPropertyStatus() const{
        return propertyStatus;
    }

    Color PropertyPlot::getColor() const {
    return color;
}

    bool PropertyPlot::isMortgaged() const{
        return propertyStatus == PropertyStatus::MORTGAGED;
    }
    void PropertyPlot::applyFestival(){
        if (festivalMultiplier < 8){
            festivalMultiplier *= 2;
        }
        festivalDuration = 3;
    }

    void PropertyPlot::endFestival(){
        festivalDuration = 0;
        festivalMultiplier = 1;
    }