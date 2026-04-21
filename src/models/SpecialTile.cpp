#include "../include/models/SpecialTile.hpp"

SpecialTile::SpecialTile(int position, const std::string& name, const std::string& color,
                         SpecialType specialType)
    : ActionTile(position, name, color, ActionType::SPECIAL), specialType(specialType) {
}

SpecialType SpecialTile::getSpecialType() const {
    return specialType;
}

void SpecialTile::setSpecialType(SpecialType type) {
    specialType = type;
}

void SpecialTile::executeAction(Player* player) {
    if (player != nullptr) {
        executeSpecial(player);
    }
}
