#include "../../include/models/PropertyTile.hpp"
#include "../../include/core/TileVisitor.hpp"
#include "../../include/models/Player.hpp"


const std::map<int, int> RailroadTile::railroad_multiplier;

const std::map<int, int> UtilityTile::utility_multiplier;

// PropertyTile
PropertyTile::PropertyTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status)
    : Tile(index, name, code, color), buy_price(buy_price), mortgage_price(mortgage_price), owner(owner), festival_level(festival_level), festival_turns_left(festival_turns_left), property_status(property_status) {}

void PropertyTile::onLand(Player& p, TileVisitor& visitor) {}

void PropertyTile::applyFestival() {
    festival_level++;
    festival_turns_left = 3;
}

void PropertyTile::decreaseFestival() {
    if (festival_turns_left > 0) festival_turns_left--;
}

int PropertyTile::getMortgageValue() const {
    return mortgage_price;
}

int PropertyTile::getBuyPrice() const {
    return buy_price;
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
    std::shared_ptr<Player> current_owner = owner.lock();
    if (!current_owner) return 0; 

    int rent_price = 0;
    int owned_count = current_owner->countOwnedRailroad();
    
    auto it = railroad_multiplier.find(owned_count);
    if (it != railroad_multiplier.end()) {
        rent_price = it->second;
    }

    if (festival_turns_left > 0) {
        rent_price *= (1 << festival_level);
    }
    
    return rent_price;
}

void RailroadTile::onLand(Player& p, TileVisitor& visitor) {
    visitor.visitRailroadTile(this, p);
}
PropertyType RailroadTile::getPropertyType() const{
    return type;
};

// UtilityTile
UtilityTile::UtilityTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status) 
    : PropertyTile(index, name, code, color, buy_price, mortgage_price, owner, festival_level, festival_turns_left, property_status) {}

int UtilityTile::calculateRent() const {
    std::shared_ptr<Player> current_owner = owner.lock();
    if (!current_owner) return 0; 

    int rent_price = 0;
    int owned_count = current_owner->countOwnedUtility();
    
    auto it = utility_multiplier.find(owned_count);
    if (it != utility_multiplier.end()) {
        rent_price = it->second;
    }

    if (festival_turns_left > 0) {
        rent_price *= (1 << festival_level);
    }
    
    return rent_price;
}

void UtilityTile::onLand(Player& p, TileVisitor& visitor) {
    visitor.visitUtilityTile(this, p);
}
PropertyType UtilityTile::getPropertyType() const{
    return type;
};