#include "PropertyTile.hpp"

// PropertyTile
PropertyTile::PropertyTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status)
    : Tile(index, name, code, color), buy_price(buy_price), mortgage_price(mortgage_price), owner(owner), festival_level(festival_level), festival_turns_left(festival_turns_left), property_status(property_status) {}

void PropertyTile::onLand(Player& p) {

}

void PropertyTile::applyFestival() {
    festival_level++;
    festival_turns_left = 3;
}

void PropertyTile::decreaseFestival() {
    if (festival_turns_left > 0) festival_turns_left--;
}

int PropertyTile::getSellValue() const {
    return buy_price;
}

int PropertyTile::getMortgageValue() const {
    return mortgage_price;
}

PropertyStatus PropertyTile::getPropertyStatus() const {
    return property_status;
}

void PropertyTile::setPropertyStatus(PropertyStatus ps) {
    property_status = ps;
}

std::weak_ptr<Player> PropertyTile::getPropertyOwner() const {
    return owner;
}

void PropertyTile::setPropertyOwner(std::shared_ptr<Player> p) {
    owner = p;
}

// RailroadTile
RailroadTile::RailroadTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status)
    : PropertyTile(index, name, code, color, buy_price, mortgage_price, owner, festival_level, festival_turns_left, property_status) {}

int RailroadTile::calculateRent() const {

}

void RailroadTile::onLand(Player& p) {

}

// UtilityTile
UtilityTile::UtilityTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status, int multiplier_factor) 
    : PropertyTile(index, name, code, color, buy_price, mortgage_price, owner, festival_level, festival_turns_left, property_status), multiplier_factor(multiplier_factor) {}

int UtilityTile::calculateRent() const {

}

void UtilityTile::onLand(Player& p) {

}