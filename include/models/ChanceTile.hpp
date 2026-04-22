#pragma once

#include "ActionTile.hpp"

// Forward declaration
class CardDeck;
class ActionCard;

class ChanceTile : public ActionTile {
public:
    ChanceTile(int position, const std::string& name, const std::string& color);
    ~ChanceTile() = default;

    ActionCard* drawCard(Player* player, CardDeck* deck);
    void executeAction(Player* player) override;
};
