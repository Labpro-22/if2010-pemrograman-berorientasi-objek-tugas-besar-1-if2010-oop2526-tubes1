#pragma once

#include <string>
#include <vector>
#include "core/services/CommandHandler.hpp"
#include "core/services/EffectResolver.hpp"
#include "core/GameState.hpp"
#include "core/TurnManager.hpp"
#include "utils/ConfigLoader.hpp"

class GameEngine {
private:
    GameState state;
    TurnManager turnManager;
    CommandHandler commandHandler;
    EffectResolver effectResolver;

public:
    GameEngine();

    void startNewGame(const std::vector<std::string>& playerNames); //TODO: handle player names di dalamnya
    void loadGameConfig();
    void loadGame(const GameState& loadedState);
    void run(const std::vector<std::string>& scriptedCommands);

    bool isGameOver() const;
    std::vector<Player> getWinner() const;

    GameState& getState();
    const GameState& getState() const;
};
