#include "../include/models/ChanceTile.hpp"
#include "../include/core/GameContext.hpp"

ChanceTile::ChanceTile(int position, const std::string& name,
                       const std::string& color, CardDeck<ActionCard>* deck)
    : ActionTile(position, name, color, ActionType::CHANCE), deck(deck) {}

ActionCard* ChanceTile::drawCard() {
    if (deck == nullptr || deck->isEmpty()) return nullptr;
    return deck->drawTop();
}

void ChanceTile::executeAction(Player* player, GameContext* ctx) {
    if (player == nullptr || ctx == nullptr) return;

    ActionCard* card = drawCard();
    if (card == nullptr) return;

    card->execute(player, ctx);
    deck->discard(card);
}