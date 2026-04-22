#pragma once
#include "ActionTile.hpp"
#include "../utils/CardDeck.hpp"
#include "ActionCard.hpp"

class GameContext;

class CommunityChestTile : public ActionTile {
private:
    CardDeck<ActionCard>* deck;

public:
    CommunityChestTile(int position, const std::string& name,
                       const std::string& color, CardDeck<ActionCard>* deck);
    ~CommunityChestTile() = default;

    ActionCard* drawCard();
    void executeAction(Player* player, GameContext* ctx) override;
};