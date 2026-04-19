#include "core/EffectResolver.hpp"

#include <stdexcept>

void EffectResolver::resolveLanding(Player& player, int tileIndex, GameState& state) const {
    state.addLog(player.getUsername() + " mendarat di tile " + std::to_string(tileIndex) + ".");
}

void EffectResolver::resolveBankruptcy(Player& player, Player* creditor, GameState& state) const {
    bankruptcyService.transferAssets(player, creditor, state);
}

void EffectResolver::resolveTax(Player& player, int taxAmount, GameState& state) const {
    if (taxAmount < 0) {
        throw std::invalid_argument("Pajak tidak boleh bernilai negatif.");
    }

    if (bankruptcyService.canRecover(player, taxAmount)) {
        player.pay(taxAmount);
        state.addLog(player.getUsername() + " membayar pajak sebesar " + std::to_string(taxAmount) + ".");
        return;
    }

    bankruptcyService.liquidateAssets(player, taxAmount - player.getCash(), state);
    if (bankruptcyService.canRecover(player, taxAmount)) {
        player.pay(taxAmount);
        state.addLog(player.getUsername() + " membayar pajak sebesar " + std::to_string(taxAmount) + " setelah likuidasi aset.");
        return;
    }

    resolveBankruptcy(player, nullptr, state);
}

bool EffectResolver::resolveAuction(int basePrice, std::vector<Player*>& bidders, Player*& winner, int& finalPrice, GameState& state) const {
    return auctionService.startAuction(basePrice, bidders, winner, finalPrice, state);
}
