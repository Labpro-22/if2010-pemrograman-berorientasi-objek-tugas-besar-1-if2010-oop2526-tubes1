#include "../include/models/GoToJailTile.hpp"
#include "../include/models/Player.hpp"

GoToJailTile::GoToJailTile(int position, const std::string& name, const std::string& color)
    : SpecialTile(position, name, color, SpecialType::GO_TO_JAIL) {
}

void GoToJailTile::sendToJail(Player* player) {
    if (player == nullptr) {
        return;
    }

    
    player->setPosition(11);
    player->setStatus(PlayerStatus::JAILED);
}

void GoToJailTile::executeSpecial(Player* player, GameContext* ctx) {
    if (player != nullptr) {
        sendToJail(player);
    }
}
