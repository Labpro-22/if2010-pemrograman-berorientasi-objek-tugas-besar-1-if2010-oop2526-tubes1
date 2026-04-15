#include "models/Plot/PropertyPlot/LandPlot.hpp"

// LandPlot();

int LandPlot::getBuyPrice() const {
    return buyPrice;
}

int LandPlot::getUpgHousePrice() const {
    return upgHotelPrice;
}

int LandPlot::getUpgHotelPrice() const {
    return upgHotelPrice;
}

int LandPlot::getRentPrice(int level) const {
    return rentPricePerLevel.at(level);
}

int LandPlot::getLevel() const {
    return level;
}

// void LandPlot::build(); //TODO:
// void LandPlot::sellBuildings(); //TODO:
// bool LandPlot::canBuild() const; //TODO:
// int LandPlot::calculateRentPrice() const override; //TODO:
// std::string LandPlot::getType() const override; //TODO:

Color LandPlot::getColor() const {
    return color;
}