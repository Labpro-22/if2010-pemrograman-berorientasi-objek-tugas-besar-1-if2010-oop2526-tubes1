#include "../include/models/Railroad.hpp"
#include "../include/models/Player.hpp"

Railroad::Railroad(int position, const std::string& name, const std::string& code, const std::string& color,
                     int purchasePrice, int mortgageValue)
    : Property(position, name, code, color, purchasePrice, mortgageValue) {
}

int Railroad::getRent(int railroadCount) const {
    auto it = rentTable.find(railroadCount);
    if (it != rentTable.end()) {
        return it->second;
    }
    return 0;
}

void Railroad::setRentTable(const std::map<int, int>& table) {
    rentTable = table;
}

int Railroad::calculateRent(Player* visitor) {
    if (status != StatusType::OWNED || visitor == nullptr) {
        return 0;
    }

    // Count how many railroads the owner has
    int railroadCount = 1; // Default to 1 if not tracked elsewhere
    int baseRent = getRent(railroadCount);
    return baseRent * festivalMultiplier;
}

void Railroad::mortgage() {
    Property::mortgage();
}

void Railroad::redeem() {
    Property::redeem();
}
