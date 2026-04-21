#include "../include/models/ActionTile.hpp"


ActionTile::ActionTile(int position, const std::string& name, const std::string& color,
                       ActionType actionType)
    : Tile(position, name, color), actionType(actionType) {
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

