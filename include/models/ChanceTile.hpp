#pragma once
#include "ActionTile.hpp"
#include "../utils/CardDeck.hpp"
#include "ActionCard.hpp"

class GameContext;

class ChanceTile : public ActionTile {
private:
    CardDeck<ActionCard>* deck;

public:
    ChanceTile(int position, const std::string& name,
               const std::string& color, CardDeck<ActionCard>* deck);
    ~ChanceTile() = default;

    ActionCard* drawCard();
    void executeAction(Player* player, GameContext* ctx) override;
};