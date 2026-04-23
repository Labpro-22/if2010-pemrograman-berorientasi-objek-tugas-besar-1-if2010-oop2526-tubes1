#include "core/Command.hpp"

#include <utility>
#include "core/GameException.hpp"
#include "core/SkillContext.hpp"

RollDiceCommand::RollDiceCommand(int boardSize) : boardSize(boardSize) {
    if (boardSize <= 0) {
        throw InvalidInputException("boardSize harus lebih dari 0.");
    }
}

bool RollDiceCommand::execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const {
    if (state.getPlayers().empty()) {
        state.addLog("Tidak ada pemain untuk melempar dadu.");
        return false;
    }

    Player& player = state.getCurrentPlayer();
    if (player.getHasRolled()) {
        state.addLog(player.getUsername() + " sudah melakukan ROLL pada giliran ini.");
        return false;
    }

    const std::pair<int, int> dicePair = state.getDice().roll();
    const int steps = dicePair.first + dicePair.second;
    const int effectiveBoardSize = state.getBoardSizeOrDefault(boardSize);
    const bool isDouble = state.getDice().isDouble();

    state.addLog(player.getUsername() + " melempar dadu: " +
        std::to_string(dicePair.first) + " dan " + std::to_string(dicePair.second) + ".");
    player.setHasRolled(true);

    if (player.getStatus() == PlayerStatus::JAILED) {
        const bool released = turnManager.handleJailedRoll(player, isDouble, state);
        if (!released) {
            return true;
        }
        player.move(steps, effectiveBoardSize);
        effectResolver.resolveLanding(player, player.getPosition(), state);
        return true;
    }

    if (isDouble) {
        if (player.getConsecutiveDoubles() + 1 >= TurnManager::MAX_CONSECUTIVE_DOUBLES) {
            state.addLog(player.getUsername() +
                " melempar double tiga kali berturut-turut dan langsung masuk penjara.");
            turnManager.sendToJail(player, state);
            return true;
        }
        player.incrementConsecutiveDoubles();
    } else {
        player.resetConsecutiveDoubles();
    }

    player.move(steps, effectiveBoardSize);
    effectResolver.resolveLanding(player, player.getPosition(), state);

    if (player.getStatus() != PlayerStatus::JAILED) {
        turnManager.handleExtraTurn(player, isDouble, state);
    }
    return true;
}

SetDiceCommand::SetDiceCommand(int dice1, int dice2) : dice1(dice1), dice2(dice2) {}

bool SetDiceCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.getDice().setDiceManual(dice1, dice2);
    state.addLog("Dadu manual diset ke " + std::to_string(dice1) + " dan " + std::to_string(dice2) + ".");
    return true;
}

bool PrintBoardCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    const Board& board = state.getBoard();
    state.addLog(
        "Perintah PRINT_BOARD dijalankan. Jumlah tile board: " +
        std::to_string(board.getSize()) + "."
    );
    return true;
}

PrintDeedCommand::PrintDeedCommand(std::string code) : code(std::move(code)) {}

bool PrintDeedCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah PRINT_DEED untuk properti " + code + " dijalankan.");
    return true;
}

bool PrintPropertyCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah PRINT_PROPERTY dijalankan.");
    return true;
}

BuildCommand::BuildCommand(std::string propertyCode) : propertyCode(std::move(propertyCode)) {}

bool BuildCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah BUILD untuk properti " + propertyCode + " dijalankan.");
    return true;
}

MortgageCommand::MortgageCommand(std::string propertyCode) : propertyCode(std::move(propertyCode)) {}

bool MortgageCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah MORTGAGE untuk properti " + propertyCode + " dijalankan.");
    return true;
}

RedeemCommand::RedeemCommand(std::string propertyCode) : propertyCode(std::move(propertyCode)) {}

bool RedeemCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah REDEEM untuk properti " + propertyCode + " dijalankan.");
    return true;
}

UseSkillCardCommand::UseSkillCardCommand(int cardIndex) : cardIndex(cardIndex) {
    if (cardIndex < 0) {
        throw InvalidInputException("cardIndex tidak boleh negatif.");
    }
}

bool UseSkillCardCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    Player& player = state.getCurrentPlayer();
    SkillContext ctx{player, state.getPlayers(), state.getBoard(), state.getLogger()};
    if (!player.useCards(static_cast<std::size_t>(cardIndex), ctx)) {
        throw InvalidInputException("Kartu skill pada index tersebut tidak dapat digunakan.");
    }

    state.addLog(player.getUsername() + " menggunakan skill card index " + std::to_string(cardIndex) + ".");
    return true;
}

SaveCommand::SaveCommand(std::string filename) : filename(std::move(filename)) {}

bool SaveCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah SAVE ke file " + filename + " dijalankan.");
    return true;
}

TaxCommand::TaxCommand(int amount) : amount(amount) {
    if (amount < 0) {
        throw InvalidInputException("Nominal pajak tidak boleh negatif.");
    }
}

bool TaxCommand::execute(GameState& state, EffectResolver& effectResolver, TurnManager&) const {
    effectResolver.resolveTax(state.getCurrentPlayer(), amount, state);
    return true;
}

EarnCommand::EarnCommand(int amount) : amount(amount) {
    if (amount < 0) {
        throw InvalidInputException("Nominal uang tidak boleh negatif.");
    }
}

bool EarnCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    Player& player = state.getCurrentPlayer();
    player.receive(amount);
    state.addLog(player.getUsername() + " menerima uang sebesar " + std::to_string(amount) + ".");
    return true;
}

BankruptCommand::BankruptCommand(int creditorIdx) : creditorIdx(creditorIdx) {}

bool BankruptCommand::execute(GameState& state, EffectResolver& effectResolver, TurnManager&) const {
    Player* creditor = nullptr;
    if (creditorIdx >= 0 && creditorIdx < static_cast<int>(state.getPlayers().size()) && creditorIdx != state.getCurrentPlayerIdx()) {
        creditor = &state.getPlayers()[static_cast<std::size_t>(creditorIdx)];
    }

    effectResolver.resolveBankruptcy(state.getCurrentPlayer(), creditor, state);
    return true;
}

bool EndTurnCommand::execute(GameState& state, EffectResolver&, TurnManager& turnManager) const {
    Player& currentPlayer = state.getCurrentPlayer();
    turnManager.endTurn(currentPlayer, state);
    turnManager.advanceTurn(state);

    if (!state.isGameOver()) {
        turnManager.startTurn(state.getCurrentPlayer(), state);
    }
    return true;
}

bool PayJailFineCommand::execute(GameState& state, EffectResolver&, TurnManager& turnManager) const {
    Player& player = state.getCurrentPlayer();
    if (player.getStatus() != PlayerStatus::JAILED) {
        state.addLog(player.getUsername() + " tidak sedang berada di penjara.");
        return false;
    }
    if (player.getHasRolled()) {
        state.addLog(player.getUsername() + " harus membayar denda sebelum melempar dadu.");
        return false;
    }
    return turnManager.payJailFine(player, state);
}
