#include "models/Plot/PropertyPlot/StationPlot.hpp"

StationPlot::StationPlot(std::string name, std::string code, Color color, int buyPrice, int mortgageValue,
                         Player* owner, PropertyStatus propertyStatus,
                         int festivalDuration, int festivalMultiplier):
    PropertyPlot(name, code, color, buyPrice, mortgageValue, owner, propertyStatus, festivalDuration, festivalMultiplier) {}

std::map<int, int> StationPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int StationPlot::getRentPrice(int level) const {
    return rentPriceTable.at(level);
}

void StationPlot::setRentPriceTable(std::map<int, int> rentPriceTable){
    rentPriceTable = rentPriceTable;
}

int StationPlot::calculateRentPrice(PlotContext& ctx) const {
    int ownedStation = owner->countOwnedStation();
    return rentPriceTable.at(ownedStation)*festivalMultiplier;
}

PlotType StationPlot::getType() const {
    return PlotType::STATIONPLOT;
}

void StationPlot::startEvent(PlotContext& ctx) {
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