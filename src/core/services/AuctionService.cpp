#include "core/services/AuctionService.hpp"

#include <stdexcept>

bool AuctionService::startAuction(
    int basePrice,
    const std::vector<Player*>& bidders,
    Player*& winner,
    int& finalPrice,
    GameState& state
) const {
    if (basePrice < 0) {
        throw std::invalid_argument("Harga dasar lelang tidak boleh negatif.");
    }

    winner = nullptr;
    finalPrice = 0;

    for (Player* bidder : bidders) {
        if (bidder == nullptr || bidder->isBankrupt() || bidder->getCash() < basePrice) {
            continue;
        }

        if (winner == nullptr || bidder->getCash() > winner->getCash()) {
            winner = bidder;
        }
    }

    if (winner == nullptr) {
        state.addLog("Lelang gagal: tidak ada pemain yang memenuhi syarat.");
        return false;
    }

    finalPrice = basePrice + (basePrice / 5);
    if (finalPrice > winner->getCash()) {
        finalPrice = winner->getCash();
    }

    winner->pay(finalPrice);
    state.addLog("Lelang dimenangkan oleh " + winner->getUsername() + " dengan harga " + std::to_string(finalPrice) + ".");
    return true;
}
