#include "../include/models/CommunityChestTile.hpp"

CommunityChestTile::CommunityChestTile(int position, const std::string& name, const std::string& color)
    : ActionTile(position, name, color, ActionType::COMMUNITY_CHEST) {
}

ActionCard* CommunityChestTile::drawCard(Player* player, CardDeck* deck) {
    if (deck.isEmpty()) {
        return nullptr;
    }
    return deck -> drawTop();
}


void CommunityChestTile::executeAction(Player* player) {
    ActionCard* card = drawCard(player);
    if (card != nullptr && player != nullptr) {
        //card implementation goes here
    }
}
