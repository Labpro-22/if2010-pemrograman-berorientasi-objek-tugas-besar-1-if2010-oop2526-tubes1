#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

PropertyPlot::PropertyPlot(std::string name, std::string code, Color color, int buyPrice, int mortgageValue, 
                            Player* owner, PropertyStatus propertyStatus,
                            int festivalDuration, int festivalMultiplier):
    Plot(name, code, color),
    buyPrice(buyPrice),
    mortgageValue(mortgageValue),
    propertyStatus(propertyStatus),
    owner(owner),
    festivalDuration(festivalDuration),
    festivalMultiplier(festivalMultiplier)
    {}

int PropertyPlot::getBuyPrice() const{
    return buyPrice;
}

int PropertyPlot::getMortgageValue() const{
    return mortgageValue;
}

PropertyStatus PropertyPlot::getPropertyStatus() const{
    return propertyStatus;
}

Player* PropertyPlot::getOwner() const  {
    return owner;
}

void PropertyPlot::setOwner(Player* player){
    owner = player;
}

bool PropertyPlot::isOwned() const{
    return owner != NULL;
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