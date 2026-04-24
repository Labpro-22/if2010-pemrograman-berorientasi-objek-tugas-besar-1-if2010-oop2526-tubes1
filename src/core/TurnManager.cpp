#include "core/TurnManager.hpp"

#include <string>
#include "core/GameException.hpp"
#include "core/services/BankruptcyService.hpp"

void TurnManager::startTurn(Player& player, GameState& state) {
    player.resetTurnFlags();
    player.resetConsecutiveDoubles();

    if (player.getStatus() == PlayerStatus::JAILED) {
        if (player.getJailTurns() <= 0) {
            state.addLog(player.getUsername() +
                " sudah melewati batas giliran di penjara, wajib membayar denda.");
            if (!payJailFine(player, state)) {
                state.addLog(player.getUsername() +
                    " tidak dapat membayar denda penjara dan dinyatakan bangkrut.");
            }
            return;
        }

        state.addLog(player.getUsername() + " berada di penjara (" +
            std::to_string(player.getJailTurns()) +
            " giliran tersisa). Pilih: PAY_JAIL_FINE atau ROLL untuk mencoba double.");
        return;
    }

    state.addLog("Mulai giliran: " + player.getUsername());
}

void TurnManager::endTurn(Player& player, GameState& state) {
    state.addLog("Selesai giliran: " + player.getUsername());
}

bool TurnManager::handleExtraTurn(Player& player, bool rolledDouble, GameState& state) {
    if (!rolledDouble) {
        return false;
    }
    if (player.isBankrupt() || player.getStatus() == PlayerStatus::JAILED) {
        return false;
    }

    player.setHasRolled(false);
    state.addLog(player.getUsername() + " mendapat extra turn setelah melempar double.");
    return true;
}

void TurnManager::advanceTurn(GameState& state) {
    state.nextPlayer();
    if (state.getCurrentTurn() > state.getMaxTurn()) {
        state.setGameOver(true);
        state.addLog("Permainan selesai karena batas giliran tercapai.");
    }
}

void TurnManager::sendToJail(Player& player, GameState& state) {
    const int boardSize = state.getBoardSizeOrDefault();
    int jailIndex = state.getBoard().findPlotIndex("PEN");
    if (jailIndex < 0 || jailIndex >= boardSize) {
        jailIndex = 0;
    }

    try {
        player.moveTo(jailIndex, boardSize);
    } catch (const GameException&) {
        // ignore move failure — status change is the important part
    }
    player.setStatus(PlayerStatus::JAILED);
    player.setJailTurns(MAX_JAIL_TURNS);
    player.resetConsecutiveDoubles();
    player.setHasRolled(true);
    state.addLog(player.getUsername() + " masuk penjara.");
}

bool TurnManager::payJailFine(Player& player, GameState& state) {
    if (player.getStatus() != PlayerStatus::JAILED) {
        return false;
    }

    BankruptcyService bankruptcyService;
    if (!bankruptcyService.canRecover(player, JAIL_FINE)) {
        bankruptcyService.liquidateAssets(player, JAIL_FINE - player.getCash(), state);
    }
    if (!bankruptcyService.canRecover(player, JAIL_FINE)) {
        bankruptcyService.transferAssets(player, nullptr, state);
        return false;
    }

    player.pay(JAIL_FINE);
    player.setStatus(PlayerStatus::ACTIVE);
    player.setJailTurns(0);
    state.addLog(player.getUsername() + " membayar denda penjara sebesar " +
        std::to_string(JAIL_FINE) + " dan bebas.");
    return true;
}

bool TurnManager::handleJailedRoll(Player& player, bool rolledDouble, GameState& state) {
    if (player.getStatus() != PlayerStatus::JAILED) {
        return true;
    }

    if (rolledDouble) {
        player.setStatus(PlayerStatus::ACTIVE);
        player.setJailTurns(0);
        player.resetConsecutiveDoubles();
        state.addLog(player.getUsername() + " bebas dari penjara karena melempar double.");
        return true;
    }

    player.decrementJailTurns();
    if (player.getJailTurns() <= 0) {
        state.addLog(player.getUsername() +
            " gagal melempar double tiga kali. Wajib membayar denda penjara.");
        if (payJailFine(player, state)) {
            return true;
        }
        return false;
    }

    state.addLog(player.getUsername() + " gagal melempar double, tetap di penjara (" +
        std::to_string(player.getJailTurns()) + " giliran tersisa).");
    return false;
}
