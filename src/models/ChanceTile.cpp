#include "../include/models/ChanceTile.hpp"

ChanceTile::ChanceTile(int position, const std::string& name, const std::string& color)
    : ActionTile(position, name, color, ActionType::CHANCE) {
}

ActionCard* ChanceTile::drawCard(Player* player, CardDeck* deck) {
    if (deck.isEmpty()) {
        return nullptr;
    }
    return deck -> drawTop();
}

