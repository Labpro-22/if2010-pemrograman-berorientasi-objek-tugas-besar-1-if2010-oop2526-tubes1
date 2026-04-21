#include "../include/models/Utility.hpp"
#include "../include/models/Player.hpp"

Utility::Utility(int position, const std::string& name, const std::string& color,
                 int purchasePrice, int mortgageValue)
    : Property(position, name, color, purchasePrice, mortgageValue) {
}

int Utility::getRent(int diceTotal, int utilityCount) const {
    auto it = multiplierTable.find(utilityCount);
    if (it != multiplierTable.end()) {
        return diceTotal * it->second;
    }
    return 0;
}

void Utility::setMultiplierTable(const std::map<int, int>& table) {
    multiplierTable = table;
}

int Utility::calculateRent(Player* visitor) {
    if (status != StatusType::OWNED || visitor == nullptr) {
        return 0;
    }

    // Dice roll will be passed from game context
    int diceTotal = 0; // This should be set from the game context
    int utilityCount = 1; // Default to 1 if not tracked elsewhere
    int baseRent = getRent(diceTotal, utilityCount);
    return baseRent * festivalMultiplier;
}

void Utility::mortgage() {
    Property::mortgage();
}

void Utility::redeem() {
    Property::redeem();
}
