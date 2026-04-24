#include "core/services/BankruptcyService.hpp"

#include "core/GameException.hpp"

bool BankruptcyService::canRecover(const Player& player, int amountNeeded) const {
    if (amountNeeded < 0) {
        throw InvalidInputException("amountNeeded tidak boleh negatif.");
    }
    return player.getCash() >= amountNeeded;
}

int BankruptcyService::liquidateAssets(Player& player, int amountNeeded, GameState& state) const {
    if (amountNeeded < 0) {
        throw InvalidInputException("amountNeeded tidak boleh negatif.");
    }
    if (amountNeeded == 0) {
        return 0;
    }

    state.addLog(player.getUsername() + " mencoba likuidasi aset, tetapi belum ada aset terdaftar untuk dijual.");
    return 0;
}

void BankruptcyService::transferAssets(Player& bankrupt, Player* creditor, GameState& state) const {
    const int remainingCash = bankrupt.getCash();
    if (creditor != nullptr && creditor != &bankrupt && !creditor->isBankrupt() && remainingCash > 0) {
        creditor->receive(remainingCash);
    }

    bankrupt.setStatus(PlayerStatus::BANKRUPT);
    state.addLog(bankrupt.getUsername() + " dinyatakan bangkrut.");
}
