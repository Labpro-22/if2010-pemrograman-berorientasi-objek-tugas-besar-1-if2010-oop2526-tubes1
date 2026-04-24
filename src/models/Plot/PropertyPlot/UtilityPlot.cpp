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
    rentPriceTable = rentPriceTable;
}

int UtilityPlot::calculateRentPrice(PlotContext& ctx) const {
    int ownedUtility = owner->countOwnedUtility();
    int diceTotal = ctx.getDice().getTotal();
    return rentPriceTable.at(ownedUtility)*diceTotal*festivalMultiplier;
}

PlotType UtilityPlot::getType() const {
    return PlotType::UTILITYPLOT;
}

void UtilityPlot::startEvent(PlotContext& ctx) {
    if (!isOwned()){
        ctx.getCurrentPlayer().buyProperty(*this);
    }
    else{
        if (owner != &ctx.getCurrentPlayer()){
            int rentPrice = calculateRentPrice(ctx);
            ctx.getCurrentPlayer().pay(rentPrice); //TODO: handle bankrupt
            owner->receive(rentPrice);
        }
    }
}
