#pragma once

#include <string>
#include <vector>
#include "core/CommandHandler.hpp"
#include "core/EffectResolver.hpp"
#include "core/GameState.hpp"
#include "core/TurnManager.hpp"

class GameEngine {
private:
    GameState state;
    TurnManager turnManager;
    CommandHandler commandHandler;
    EffectResolver effectResolver;

public:
    GameEngine();

    void startNewGame(const std::vector<std::string>& playerNames, int startingCash = 1500, int maxTurn = 100);
    void loadGame(const GameState& loadedState);
    void run(const std::vector<std::string>& scriptedCommands);

    bool isGameOver() const;
    std::vector<Player> getWinner() const;

    GameState& getState();
    const GameState& getState() const;
};
