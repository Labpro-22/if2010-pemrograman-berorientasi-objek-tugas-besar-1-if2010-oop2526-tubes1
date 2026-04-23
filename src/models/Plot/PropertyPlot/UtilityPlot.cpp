#include "models/Plot/PropertyPlot/UtilityPlot.hpp"

UtilityPlot::UtilityPlot(std::string name, std::string code, Color color, int mortgageValue,
                        std::map<int, int> rentPriceTable, PropertyStatus propertyStatus = PropertyStatus::BANK):
    PropertyPlot(name, code, color, mortgageValue, propertyStatus) {}

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
    int ownedUtility = 0; //TODO: dummy
    int diceTotal = 6; //TODO: dummy
    return rentPriceTable.at(ownedUtility)*diceTotal*festivalMultiplier;
}

std::string UtilityPlot::getType() const {
    return "Petak Utilitas";
}

void UtilityPlot::startEvent(PlotContext& ctx) {

}
