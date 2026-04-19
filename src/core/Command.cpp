#include "core/Command.hpp"

#include <stdexcept>
#include <utility>

RollDiceCommand::RollDiceCommand(int boardSize) : boardSize(boardSize) {
    if (boardSize <= 0) {
        throw std::invalid_argument("boardSize harus lebih dari 0.");
    }
}

bool RollDiceCommand::execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const {
    if (state.getPlayers().empty()) {
        state.addLog("Tidak ada pemain untuk melempar dadu.");
        return false;
    }

    Player& player = state.getCurrentPlayer();
    const std::pair<int, int> dicePair = state.getDice().roll();
    const int steps = dicePair.first + dicePair.second;

    state.addLog(player.getUsername() + " melempar dadu: " + std::to_string(dicePair.first) + " dan " + std::to_string(dicePair.second) + ".");
    player.move(steps, boardSize);
    effectResolver.resolveLanding(player, player.getPosition(), state);

    turnManager.handleExtraTurn(player, state.getDice().isDouble(), state);
    return true;
}

SetDiceCommand::SetDiceCommand(int die1, int die2) : die1(die1), die2(die2) {}

bool SetDiceCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.getDice().setManual(die1, die2);
    state.addLog("Dadu manual diset ke " + std::to_string(die1) + " dan " + std::to_string(die2) + ".");
    return true;
}

bool PrintBoardCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    state.addLog("Perintah PRINT_BOARD dijalankan.");
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
        throw std::invalid_argument("cardIndex tidak boleh negatif.");
    }
}

bool UseSkillCardCommand::execute(GameState& state, EffectResolver&, TurnManager&) const {
    Player& player = state.getCurrentPlayer();
    if (player.hasUsedSkillThisTurn()) {
        throw std::invalid_argument("Player sudah menggunakan skill pada giliran ini.");
    }

    player.setUsedSkillThisTurn(true);
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
        throw std::invalid_argument("Nominal pajak tidak boleh negatif.");
    }
}

bool TaxCommand::execute(GameState& state, EffectResolver& effectResolver, TurnManager&) const {
    effectResolver.resolveTax(state.getCurrentPlayer(), amount, state);
    return true;
}

EarnCommand::EarnCommand(int amount) : amount(amount) {
    if (amount < 0) {
        throw std::invalid_argument("Nominal uang tidak boleh negatif.");
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
