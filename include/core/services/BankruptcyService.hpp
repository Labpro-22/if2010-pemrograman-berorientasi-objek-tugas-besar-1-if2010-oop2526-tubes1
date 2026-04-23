#pragma once

#include "core/GameState.hpp"

class BankruptcyService {
public:
    bool canRecover(const Player& player, int amountNeeded) const;
    int liquidateAssets(Player& player, int amountNeeded, GameState& state) const;
    void transferAssets(Player& bankrupt, Player* creditor, GameState& state) const;
};

using BankcruptService = BankruptcyService;
