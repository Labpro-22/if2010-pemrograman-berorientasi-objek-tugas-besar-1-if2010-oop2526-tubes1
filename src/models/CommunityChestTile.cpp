#include "../include/models/CommunityChestTile.hpp"
#include "../include/core/GameContext.hpp"

CommunityChestTile::CommunityChestTile(int position, const std::string& name,
                                        const std::string& color, CardDeck<ActionCard>* deck)
    : ActionTile(position, name, color, ActionType::COMMUNITY_CHEST), deck(deck) {}

ActionCard* CommunityChestTile::drawCard() {
    if (deck == nullptr || deck->isEmpty()) return nullptr;
    return deck->drawTop();
}

void CommunityChestTile::executeAction(Player* player, GameContext* ctx) {
    if (player == nullptr || ctx == nullptr) return;

    ActionCard* card = drawCard();
    if (card == nullptr) return;

    card->execute(player, ctx);
    deck->discard(card);
}