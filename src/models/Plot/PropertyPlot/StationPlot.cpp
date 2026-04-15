#include "models/Plot/PropertyPlot/StationPlot.hpp"

// StationPlot(){
    
// }

std::map<int, int> StationPlot::getRentPrice() const {
    return rentPrice;
}

void StationPlot::setRentPrice(std::map<int, int> rentPriceTable){
    rentPrice = rentPriceTable;
}

int StationPlot::calculateRentPrice() const {
    int ownedStation = 0; //TODO: dummy
    auto it = rentPrice.find(ownedStation);
    return rentPrice.at(ownedStation);
}

std::string StationPlot::getType() const {
    return "Petak Stasiun";
}

Color StationPlot::getColor() const {
    return Color::DEFAULT;
}