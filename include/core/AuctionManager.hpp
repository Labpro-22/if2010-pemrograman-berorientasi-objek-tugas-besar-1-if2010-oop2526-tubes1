#pragma once

#include <vector>

class Game;
class Player;
class PropertyTile;

class AuctionManager {
public:
    AuctionManager() = default;
    ~AuctionManager() = default;

    void runAuction(PropertyTile& property, std::vector<Player*>& bidders, Player& trigger, Game& game);
    bool validateBid(const Player& player, int amount) const;
    Player* determineWinner();
    void finalizeAuction();
};