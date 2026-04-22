#include "../include/models/ActionTile.hpp"


ActionTile::ActionTile(int position, const std::string& name, const std::string& code, const std::string& color,
                       ActionType actionType)
    : Tile(position, name, code, color), actionType(actionType) {
}

ActionType ActionTile::getActionType() const {
    return actionType;
}

void ActionTile::setActionType(ActionType type) {
    actionType = type;
}

void ActionTile::onLand(Player* player) {
    if (player != nullptr) {
        executeAction(player);
    }
}


void ActionTile::onPass(Player* player) {
    (void)player;
    // Default implementation does nothing when passing
}

std::string getCode() {
    return "DF"; // default aksi
}

std::string getCategory() {
    return "DEFAULT";
}