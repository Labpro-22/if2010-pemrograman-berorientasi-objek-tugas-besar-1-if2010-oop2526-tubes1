#pragma once

#include <vector>
#include "core/services/AuctionService.hpp"
#include "core/services/BankruptcyService.hpp"
#include "core/GameState.hpp"

class EffectResolver {
private:
    AuctionService auctionService;
    BankruptcyService bankruptcyService;

public:
    void resolveLanding(Player& player, int tileIndex, GameState& state);
    void resolveBankruptcy(Player& player, Player* creditor, GameState& state) const;
    void resolveTax(Player& player, int taxAmount, GameState& state) const;
    bool resolveAuction(int basePrice, std::vector<Player*>& bidders, Player*& winner, int& finalPrice, GameState& state) const;
};
