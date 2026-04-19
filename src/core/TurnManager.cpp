#include "core/TurnManager.hpp"

void TurnManager::startTurn(Player& player, GameState& state) {
    player.resetTurnFlags();

    if (player.getStatus() == PlayerStatus::JAILED) {
        player.decrementJailTurns();
        if (player.getJailTurns() == 0) {
            player.setStatus(PlayerStatus::ACTIVE);
            state.addLog(player.getUsername() + " bebas dari penjara.");
        } else {
            state.addLog(player.getUsername() + " masih di penjara.");
        }
        return;
    }

    state.addLog("Mulai giliran: " + player.getUsername());
}

void TurnManager::endTurn(Player& player, GameState& state) {
    state.addLog("Selesai giliran: " + player.getUsername());
}

bool TurnManager::handleExtraTurn(Player& player, bool rolledDouble, GameState& state) {
    if (rolledDouble && !player.isBankrupt()) {
        state.addLog(player.getUsername() + " mendapat extra turn.");
        return true;
    }
    return false;
}

void TurnManager::advanceTurn(GameState& state) {
    state.nextPlayer();
    if (state.getCurrentTurn() > state.getMaxTurn()) {
        state.setGameOver(true);
        state.addLog("Permainan selesai karena batas giliran tercapai.");
    }
}
