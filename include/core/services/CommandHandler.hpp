#pragma once

#include <memory>
#include <string>
#include "core/Command.hpp"

class CommandHandler {
public:
    std::unique_ptr<Command> parse(const std::string& rawCommand) const;
    bool execute(const Command& command, GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const;
    static std::string promptInput(std::string prompt);
    static bool promptYesNo(std::string prompt);
};
