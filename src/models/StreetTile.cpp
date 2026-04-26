#include "../../include/models/StreetTile.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/core/GameManager.hpp"
StreetTile::StreetTile(int index, std::string name, std::string code, std::string color, float buy_price, float mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status, int building_level, const std::vector<float>& rent_list, float house_price, float hotel_price) 
    : PropertyTile(index, name, code, color, buy_price, mortgage_price, owner, festival_level, festival_turns_left, property_status), building_level(building_level), rent_list(rent_list), house_price(house_price), hotel_price(hotel_price) {}

PropertyType StreetTile::getPropertyType() const {
    return PropertyType::STREET; 
}

float StreetTile::calculateRent() const {
    if (owner.expired()) {
        return 0; 
    }
    if (property_status == PropertyStatus::MORTGAGED) {
        return 0; 
    }
    if (building_level >= 0 && building_level < rent_list.size()) {
        float base_rent = rent_list[building_level];
        
        if (festival_turns_left > 0) {
            base_rent *= (1 << festival_level);
        }
        
        return base_rent;
    }
    
    return 0; 
}   

void StreetTile::onLand(Player& p) {
    GameManager::visitStreetTile(this, p);
}

int StreetTile::getBuildingLevel() const {
    return building_level;
}

bool StreetTile::canBuildHouse() const {
    return building_level < 4;
}

bool StreetTile::canBuildHotel() const {
    return building_level == 4;
}

bool StreetTile::canSellBuilding() const {
    return building_level > 0;
}

void StreetTile::buildHouse() {
    if (canBuildHouse()) {
        building_level++;
    }
}

void StreetTile::buildHotel() {
    if (canBuildHotel()) {
        building_level++; 
    }
}

void StreetTile::sellBuilding() {
    if (canSellBuilding()) {
        building_level--; 
    }
}

float StreetTile::getHousePrice() const {
    return house_price;
}

float StreetTile::getHotelPrice() const {
    return hotel_price;
}