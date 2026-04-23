#include "../include/models/ChanceTile.hpp"

ChanceTile::ChanceTile(int position, const std::string& name, const std::string& code, const std::string& color)
    : ActionTile(position, name, "KSP", "DEFAULT", ActionType::CHANCE) {
}

ActionCard* ChanceTile::drawCard(Player* player, CardDeck<ActionCard>* deck) {
    (void)player;

    if (deck == nullptr) return nullptr;
}

void ChanceTile::executeAction(Player* player) {
    ActionCard* card = drawCard(player, nullptr);
    if (card != nullptr && player != nullptr) {
        //card implementation goes here
    }
}

