#pragma once

#include <vector>
#include "core/GameState.hpp"

class AuctionService {
public:
    bool startAuction(
        int basePrice,
        const std::vector<Player*>& bidders,
        Player*& winner,
        int& finalPrice,
        GameState& state
    ) const;
};
