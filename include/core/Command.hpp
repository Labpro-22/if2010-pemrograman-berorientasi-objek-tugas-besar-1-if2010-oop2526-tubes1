#pragma once

#include <string>
#include "core/services/EffectResolver.hpp"
#include "core/GameState.hpp"
#include "core/TurnManager.hpp"

class Command {
public:
    virtual ~Command() = default;
    virtual bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const = 0;
};

class RollDiceCommand : public Command {
private:
    int boardSize;

public:
    explicit RollDiceCommand(int boardSize = 40);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class SetDiceCommand : public Command {
private:
    int dice1;
    int dice2;

public:
    SetDiceCommand(int dice1, int dice2);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class PrintBoardCommand : public Command {
public:
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class PrintDeedCommand : public Command {
private:
    std::string code;

public:
    explicit PrintDeedCommand(std::string code);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class PrintPropertyCommand : public Command {
public:
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class BuildCommand : public Command {
private:
    std::string propertyCode;

public:
    explicit BuildCommand(std::string propertyCode);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class MortgageCommand : public Command {
private:
    std::string propertyCode;

public:
    explicit MortgageCommand(std::string propertyCode);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class RedeemCommand : public Command {
private:
    std::string propertyCode;

public:
    explicit RedeemCommand(std::string propertyCode);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class UseSkillCardCommand : public Command {
private:
    int cardIndex;

public:
    explicit UseSkillCardCommand(int cardIndex);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class SaveCommand : public Command {
private:
    std::string filename;

public:
    explicit SaveCommand(std::string filename);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class TaxCommand : public Command {
private:
    int amount;

public:
    explicit TaxCommand(int amount);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class EarnCommand : public Command {
private:
    int amount;

public:
    explicit EarnCommand(int amount);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class BankruptCommand : public Command {
private:
    int creditorIdx;

public:
    explicit BankruptCommand(int creditorIdx = -1);
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};

class EndTurnCommand : public Command {
public:
    bool execute(GameState& state, EffectResolver& effectResolver, TurnManager& turnManager) const override;
};
