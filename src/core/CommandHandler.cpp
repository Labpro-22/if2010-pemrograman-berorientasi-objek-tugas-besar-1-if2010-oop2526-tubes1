#include "core/CommandHandler.hpp"

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
std::string toUpper(std::string text) {
    for (char& ch : text) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return text;
}

int parseInt(const std::string& value) {
    std::size_t parsedCount = 0;
    const int result = std::stoi(value, &parsedCount);
    if (parsedCount != value.size()) {
        throw std::invalid_argument("Argumen numerik tidak valid: " + value);
    }
    return result;
}
}  // namespace

std::unique_ptr<Command> CommandHandler::parse(const std::string& rawCommand) const {
    std::istringstream iss(rawCommand);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        throw std::invalid_argument("Perintah kosong.");
    }

    const std::string cmd = toUpper(tokens[0]);
    if (cmd == "ROLL") {
        const int boardSize = (tokens.size() > 1) ? parseInt(tokens[1]) : 40;
        return std::make_unique<RollDiceCommand>(boardSize);
    }
    if (cmd == "SETDICE" || cmd == "SET_DICE") {
        if (tokens.size() < 3) {
            throw std::invalid_argument("SETDICE membutuhkan 2 argumen angka.");
        }
        return std::make_unique<SetDiceCommand>(parseInt(tokens[1]), parseInt(tokens[2]));
    }
    if (cmd == "PRINT_BOARD") {
        return std::make_unique<PrintBoardCommand>();
    }
    if (cmd == "PRINT_DEED") {
        if (tokens.size() < 2) {
            throw std::invalid_argument("PRINT_DEED membutuhkan kode properti.");
        }
        return std::make_unique<PrintDeedCommand>(tokens[1]);
    }
    if (cmd == "PRINT_PROPERTY") {
        return std::make_unique<PrintPropertyCommand>();
    }
    if (cmd == "BUILD") {
        if (tokens.size() < 2) {
            throw std::invalid_argument("BUILD membutuhkan kode properti.");
        }
        return std::make_unique<BuildCommand>(tokens[1]);
    }
    if (cmd == "MORTGAGE") {
        if (tokens.size() < 2) {
            throw std::invalid_argument("MORTGAGE membutuhkan kode properti.");
        }
        return std::make_unique<MortgageCommand>(tokens[1]);
    }
    if (cmd == "REDEEM") {
        if (tokens.size() < 2) {
            throw std::invalid_argument("REDEEM membutuhkan kode properti.");
        }
        return std::make_unique<RedeemCommand>(tokens[1]);
    }
    if (cmd == "USE_SKILL") {
        if (tokens.size() < 2) {
            throw std::invalid_argument("USE_SKILL membutuhkan index kartu.");
        }
        return std::make_unique<UseSkillCardCommand>(parseInt(tokens[1]));
    }
    if (cmd == "SAVE") {
        if (tokens.size() < 2) {
            throw std::invalid_argument("SAVE membutuhkan nama file.");
        }
        return std::make_unique<SaveCommand>(tokens[1]);
    }
    if (cmd == "TAX") {
        if (tokens.size() < 2) {
            throw std::invalid_argument("TAX membutuhkan nominal pajak.");
        }
        return std::make_unique<TaxCommand>(parseInt(tokens[1]));
    }
    if (cmd == "EARN") {
        if (tokens.size() < 2) {
            throw std::invalid_argument("EARN membutuhkan nominal uang.");
        }
        return std::make_unique<EarnCommand>(parseInt(tokens[1]));
    }
    if (cmd == "BANKRUPT" || cmd == "BANKCRUPT") {
        const int creditorIdx = (tokens.size() > 1) ? parseInt(tokens[1]) : -1;
        return std::make_unique<BankruptCommand>(creditorIdx);
    }
    if (cmd == "END") {
        return std::make_unique<EndTurnCommand>();
    }

    throw std::invalid_argument("Perintah tidak dikenali: " + tokens[0]);
}

bool CommandHandler::execute(const Command& command, GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const {
    return command.execute(state, effectResolver, turnManager);
}
