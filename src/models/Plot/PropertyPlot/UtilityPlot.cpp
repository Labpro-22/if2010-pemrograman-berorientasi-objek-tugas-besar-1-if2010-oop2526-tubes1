#include "models/Plot/PropertyPlot/UtilityPlot.hpp"

// UtilityPlot(){
    
// }

std::map<int, int> UtilityPlot::getRentPrice() const {
    return rentPrice;
}

void UtilityPlot::setRentPrice(std::map<int, int> rentPriceTable){
    rentPrice = rentPriceTable;
}

int UtilityPlot::calculateRentPrice() const {
    int ownedUtility = 0; //TODO: dummy
    int diceTotal = 6; //TODO: dummy
    auto it = rentPrice.find(ownedUtility);
    return rentPrice.at(ownedUtility)*diceTotal;
}

std::string UtilityPlot::getType() const {
    return "Petak Utilitas";
}

Color UtilityPlot::getColor() const {
    return Color::GRAY;
}
