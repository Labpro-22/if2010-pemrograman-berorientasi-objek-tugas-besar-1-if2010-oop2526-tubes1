#include "../include/models/CommunityChestTile.hpp"
#include "../include/utils/CardDeck.hpp"

CommunityChestTile::CommunityChestTile(int position, const std::string& name, const std::string& code, const std::string& color)
    : ActionTile(position, name, "DNU", "DEFAULT", ActionType::COMMUNITY_CHEST) {
}

ActionCard* CommunityChestTile::drawCard(Player* player, CardDeck<ActionCard>* deck) {
    (void)player;
    if (deck->isEmpty()) {
        return nullptr;
    }
}


void CommunityChestTile::executeAction(Player* player) {
    ActionCard* card = drawCard(player, nullptr);
    if (card != nullptr && player != nullptr) {
        //card implementation goes here
    }
}
