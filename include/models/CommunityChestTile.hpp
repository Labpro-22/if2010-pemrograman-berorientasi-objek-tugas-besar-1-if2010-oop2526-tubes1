#pragma once

#include "ActionTile.hpp"
#include <vector>

// Forward declaration
class ActionCard;
class CardDeck;

class CommunityChestTile : public ActionTile {


public:
    CommunityChestTile(int position, const std::string& name, const std::string& color);
    ~CommunityChestTile() = default;

    ActionCard* drawCard(Player* player, CardDeck* deck);


    void executeAction(Player* player) override;
};
