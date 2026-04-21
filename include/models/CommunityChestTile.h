#pragma once

#include "ActionTile.hpp"
#include <vector>

// Forward declaration
class ActionCard;

class CommunityChestTile : public ActionTile {
private:
    std::vector<ActionCard*> deck;

public:
    CommunityChestTile(int position, const std::string& name, const std::string& color);
    ~CommunityChestTile() = default;

    ActionCard* drawCard(Player* player);
    void addCard(ActionCard* card);
    void clearCardPool();

    void executeAction(Player* player) override;
};
