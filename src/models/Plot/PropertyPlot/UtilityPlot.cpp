#include "models/Plot/PropertyPlot/UtilityPlot.hpp"

UtilityPlot::UtilityPlot(std::string name, std::string code, Color color, int buyPrice, int mortgageValue,
                         Player* owner, PropertyStatus propertyStatus,
                         int festivalDuration, int festivalMultiplier):
    PropertyPlot(name, code, color, buyPrice, mortgageValue, owner, propertyStatus, festivalDuration, festivalMultiplier) {}

std::map<int, int> UtilityPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int UtilityPlot::getRentPrice(int level) const {
    return rentPriceTable.at(level);
}

void UtilityPlot::setRentPriceTable(std::map<int, int> rentPriceTable){
    this->rentPriceTable = rentPriceTable;
}

int UtilityPlot::calculateRentPrice() const {
    int ownedUtility = 0; //TODO: need helper function
    int diceTotal = 6; //TODO: need helper function
    return rentPriceTable.at(ownedUtility)*diceTotal*festivalMultiplier;
}

std::string UtilityPlot::getType() const {
    return "Petak Utilitas";
}

void UtilityPlot::startEvent(PlotContext& ctx) {
    if (!isOwned()){
        ctx.getCurrentPlayer().buyProperty(*this);
    }
    else{
        //TODO: bayar
    }
}
