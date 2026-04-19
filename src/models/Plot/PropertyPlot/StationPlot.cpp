#include "models/Plot/PropertyPlot/StationPlot.hpp"

StationPlot::StationPlot(std::string name, std::string code, Color color, int mortgageValue,
                        std::map<int, int> rentPriceTable, PropertyStatus propertyStatus = PropertyStatus::BANK):
    PropertyPlot(name, code, color, mortgageValue, propertyStatus) {}

std::map<int, int> StationPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int StationPlot::getRentPrice(int level) const {
    return rentPriceTable.at(level);
}

void StationPlot::setRentPriceTable(std::map<int, int> rentPriceTable){
    this->rentPriceTable = rentPriceTable;
}

int StationPlot::calculateRentPrice() const {
    int ownedStation = 0; //TODO: dummy
    return rentPriceTable.at(ownedStation)*festivalMultiplier;
}

std::string StationPlot::getType() const {
    return "Petak Stasiun";
}