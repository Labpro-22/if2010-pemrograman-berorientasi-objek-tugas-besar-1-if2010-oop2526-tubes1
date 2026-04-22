#pragma once

#include "ActionTile.hpp"
#include "../utils/CardDeck.hpp"
#include "../include/utils/ActionCard.hpp"
#include <vector>

// Forward declaration
class ActionCard;

class CommunityChestTile : public ActionTile {


public:
    CommunityChestTile(int position, const std::string& name, const std::string& code, const std::string& color);
    ~CommunityChestTile() = default;

    ActionCard* drawCard(Player* player, CardDeck<ActionCard>* deck);


    void executeAction(Player* player) override;
};
