#pragma once

#include "ActionTile.hpp"
#include "../utils/CardDeck.hpp"
#include "../include/utils/ActionCard.hpp"
#include "../include/utils/CardDeck.hpp"
#include "../include/models/Player.hpp"

// Forward declaration

class ChanceTile : public ActionTile {
public:
    ChanceTile(int position, const std::string& name, const std::string& color);
    ~ChanceTile() = default;

    ActionCard* drawCard(Player* player, CardDeck<ActionCard>* deck);
    void executeAction(Player* player) override;
};
