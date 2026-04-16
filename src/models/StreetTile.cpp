#include "StreetTile.hpp"

StreetTile::StreetTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status, int house_count, std::vector<int> rent_list, int house_price, int hotel_price) : PropertyTile(index, name, code, color, buy_price, mortgage_price, owner, festival_level, festival_turns_left, property_status), house_count(house_count), rent_list(rent_list), house_price(house_price), hotel_price(hotel_price) {}

int StreetTile::calculateRent() const {

}

void StreetTile::onLand(Player& p) {

}

bool StreetTile::canBuildHouse() const {

}

bool StreetTile::canBuildHotel() const {

}

void StreetTile::buildHouse() {

}

void StreetTile::buildHotel() {

}