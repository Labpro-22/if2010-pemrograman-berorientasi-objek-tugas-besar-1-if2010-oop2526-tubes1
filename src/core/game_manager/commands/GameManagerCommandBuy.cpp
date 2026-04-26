#include "core/game_manager/GameManager.hpp"

#include <stdexcept>

#include "models/base/Property.hpp"
#include "models/tiles/Street.hpp"

void GameManager::commandBuy() {
    Player& player = getCurrentPlayer();
    Property* property = dynamic_cast<Property*>(board.getTile(player.getPosition()));
    if (property == 0 || property->getStatus() != PropertyStatus::BANK) {
        throw std::runtime_error("No buyable property on current tile.");
    }

    Street* street = dynamic_cast<Street*>(property);
    if (street != 0) {
        handleStreetPurchase(player, *street);
        return;
    }

    property->setOwner(&player);
    player.addProperty(property);
    logger.log(currentTurn, player.getName(), "AUTO_CLAIM", property->getCode());
}
