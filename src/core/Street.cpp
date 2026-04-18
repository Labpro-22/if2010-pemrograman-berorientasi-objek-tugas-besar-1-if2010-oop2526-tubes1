#include "../../include/core/Tile.hpp"

Street::Street(int index, const std::string& code, const std::string& color, int landCost, int mortgageValue, int festivalMultiplier, int festivalDuration, Player* owner, PROPERTY_STATUS propertyStatus, int houseCost, int hotelCost, std::vector<int> rentCost, int currentLevel) : Property(index, code, color, landCost, mortgageValue, festivalMultiplier, festivalDuration, owner, propertyStatus), houseCost(houseCost), hotelCost(hotelCost), rentCost(rentCost), currentLevel(currentLevel) {} 

int Street::getHouseCost() const {
    return houseCost;
}
int Street::getHotelCost() const {
    return hotelCost;
}
int Street::getCurrentLevel() const {
    return currentLevel;
}
void Street::setCurrentLevel(int currentLevel) {
    this->currentLevel = currentLevel;
}
// placeholder for now
// void Street::runTile(Player*) {

// }
// int Street::getRentCost(Player*) {
//     return 0;
// }