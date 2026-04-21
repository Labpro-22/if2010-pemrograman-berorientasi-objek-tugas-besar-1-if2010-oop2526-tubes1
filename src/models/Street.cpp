#include "../include/models/Street.hpp"
#include "../include/models/Player.hpp"

Street::Street(int position, const std::string& name, const std::string& color,
               ColorGroup colorGroup, int purchasePrice, int mortgageValue,
               int housePrice, int hotelPrice)
    : Property(position, name, color, purchasePrice, mortgageValue),
      colorGroup(colorGroup),
      housePrice(housePrice),
      hotelPrice(hotelPrice),
      buildingCount("0") {
}

ColorGroup Street::getColorGroup() const {
    return colorGroup;
}

int Street::getRent(int houseCount) const {
    auto it = rentTable.find(houseCount);
    if (it != rentTable.end()) {
        return it->second;
    }
    return 0;
}

int Street::getHousePrice() const {
    return housePrice;
}

int Street::getHotelPrice() const {
    return hotelPrice;
}

std::string Street::getBuildingCount() const {
    return buildingCount;
}

void Street::setRentTable(const std::map<int, int>& table) {
    rentTable = table;
}

void Street::setBuildingCount(const std::string& count) {
    buildingCount = count;
}

bool Street::canBuildHouse(Player* player) const {
    if (player == nullptr || status != StatusType::OWNED) {
        return false;
    }
    return player->getMoney() >= housePrice;
}

bool Street::canBuildHotel(Player* player) const {
    if (player == nullptr || status != StatusType::OWNED) {
        return false;
    }
    return player->getMoney() >= hotelPrice;
}

void Street::buildHouse() {
    // Building logic will be managed by BuildingManager
}

void Street::buildHotel() {
    // Building logic will be managed by BuildingManager
}

void Street::sellBuilding() {
    // Selling logic will be managed by BuildingManager
}

int Street::calculateRent(Player* visitor) {
    if (status != StatusType::OWNED || visitor == nullptr) {
        return 0;
    }

    int houseCount = 0;
    try {
        houseCount = std::stoi(buildingCount);
    } catch (...) {
        houseCount = 0;
    }

    int baseRent = getRent(houseCount);
    return baseRent * festivalMultiplier;
}

void Street::mortgage() {
    Property::mortgage();
}

void Street::redeem() {
    Property::redeem();
}
