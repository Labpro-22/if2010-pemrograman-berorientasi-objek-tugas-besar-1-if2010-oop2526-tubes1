#include "core/GameEngine.hpp"

#include <stdexcept>

GameEngine::GameEngine() : state(100) {}

void GameEngine::startNewGame(const std::vector<std::string>& playerNames, int startingCash, int maxTurn) {
    if (playerNames.empty()) {
        throw std::invalid_argument("Minimal harus ada satu pemain.");
    }
    if (startingCash < 0) {
        throw std::invalid_argument("startingCash tidak boleh negatif.");
    }

    state = GameState(maxTurn);
    for (const std::string& name : playerNames) {
        state.addPlayer(Player(name, startingCash));
    }

    turnManager.startTurn(state.getCurrentPlayer(), state);
}

void GameEngine::loadGame(const GameState& loadedState) {
    state = loadedState;
}

void GameEngine::run(const std::vector<std::string>& scriptedCommands) {
    for (const std::string& rawCommand : scriptedCommands) {
        if (state.isGameOver()) {
            break;
        }

        const auto command = commandHandler.parse(rawCommand);
        commandHandler.execute(*command, state, effectResolver, turnManager);

        if (state.hasSingleActivePlayer()) {
            state.setGameOver(true);
        }
    }
}

bool GameEngine::isGameOver() const {
    return state.isGameOver();
}

std::vector<Player> GameEngine::getWinner() const {
    std::vector<Player> winners;
    const auto& players = state.getPlayers();
    if (players.empty()) {
        return winners;
    }

    const auto activePlayers = state.getActivePlayers();
    if (activePlayers.empty()) {
        return winners;
    }
    if (activePlayers.size() == 1) {
        winners.push_back(activePlayers.front());
        return winners;
    }

    int highestCash = -1;
    for (const Player& player : activePlayers) {
        if (player.getCash() > highestCash) {
            highestCash = player.getCash();
        }
    }

    for (const Player& player : activePlayers) {
        if (player.getCash() == highestCash) {
            winners.push_back(player);
        }
    }

    return winners;
}

GameState& GameEngine::getState() {
    return state;
}

const GameState& GameEngine::getState() const {
    return state;
}
