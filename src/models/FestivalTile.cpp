#include "../include/models/FestivalTile.hpp"


FestivalTile::FestivalTile(int position, const std::string& name, const std::string& color)
    : ActionTile(position, name, color, ActionType::FESTIVAL) {
}

void FestivalTile::applyFestivalEffect(Player* player, Property* property) {
    if (player == nullptr || property == nullptr) {
        return;
    }

    if (property->getFestivalDuration() == 0) {
        property->setFestivalMultiplier(2);
        property->setFestivalDuration(3); 
    }
}

void FestivalTile::updateDuration() {
    // Update logic for festival duration tracking
}

void FestivalTile::executeAction(Player* player) {
    if (player != nullptr) {
        // Apply festival effect to all properties owned by the player
        // This will be coordinated with the game controller
    }
}
