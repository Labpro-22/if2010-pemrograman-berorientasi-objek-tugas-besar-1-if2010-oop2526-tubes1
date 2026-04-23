#include "core/GameState.hpp"

#include <stdexcept>

GameState::GameState()
    : currentPlayerIdx(0),
      maxTurn(0),
      currentTurn(1),
      gameOver(false),
      board(std::make_shared<Board>()) {
    if (maxTurn <= 0) {
        throw std::invalid_argument("maxTurn harus lebih dari 0.");
    }
}

void GameState::addPlayer(const Player& player) {
    players.push_back(player);
    clampCurrentPlayerIndex();
}

std::vector<Player>& GameState::getPlayers() {
    return players;
}

const std::vector<Player>& GameState::getPlayers() const {
    return players;
}

Player& GameState::getCurrentPlayer() {
    if (players.empty()) {
        throw std::runtime_error("Tidak ada pemain dalam game state.");
    }
    clampCurrentPlayerIndex();
    return players[static_cast<std::size_t>(currentPlayerIdx)];
}

const Player& GameState::getCurrentPlayer() const {
    if (players.empty()) {
        throw std::runtime_error("Tidak ada pemain dalam game state.");
    }

    int safeIdx = currentPlayerIdx;
    if (safeIdx < 0 || safeIdx >= static_cast<int>(players.size())) {
        safeIdx = 0;
    }
    return players[static_cast<std::size_t>(safeIdx)];
}

int GameState::getCurrentPlayerIdx() const {
    return currentPlayerIdx;
}

int GameState::getMaxTurn() const {
    return maxTurn;
}

int GameState::getCurrentTurn() const {
    return currentTurn;
}

int GameState::getSalary() const{
    return salary;
}

int GameState::getJailFine() const{
    return jailFine;
}

int GameState::getStartingCash() const{
    return startingCash;
}

void GameState::setCurrentPlayerIdx(int idx) {
    currentPlayerIdx = idx;
    clampCurrentPlayerIndex();
}

void GameState::setMaxTurn(int maxTurn){
    this->maxTurn = maxTurn;
}

void GameState::setCurrentTurn(int turn) {
    if (turn <= 0) {
        throw std::invalid_argument("currentTurn harus lebih dari 0.");
    }
    currentTurn = turn;
    if (currentTurn > maxTurn) {
        gameOver = true;
    }
}

void GameState::setSalary(int amount){
    salary = amount;
}

void GameState::setJailFine(int amount){
    jailFine = amount;
}

void GameState::setStartingCash(int amount){
    startingCash = amount;
}

void GameState::nextPlayer() {
    if (players.empty()) {
        gameOver = true;
        return;
    }

    if (countActivePlayers() <= 1) {
        gameOver = true;
        return;
    }

    const int startingIdx = currentPlayerIdx;
    do {
        currentPlayerIdx = (currentPlayerIdx + 1) % static_cast<int>(players.size());
        if (currentPlayerIdx == 0) {
            ++currentTurn;
        }

        if (!players[static_cast<std::size_t>(currentPlayerIdx)].isBankrupt()) {
            return;
        }
    } while (currentPlayerIdx != startingIdx);

    gameOver = true;
}

std::vector<Player> GameState::getActivePlayers() const {
    std::vector<Player> activePlayers;
    activePlayers.reserve(static_cast<std::size_t>(countActivePlayers()));
    for (const Player& player : players) {
        if (!player.isBankrupt()) {
            activePlayers.push_back(player);
        }
    }
    return activePlayers;
}

bool GameState::hasSingleActivePlayer() const {
    return countActivePlayers() == 1;
}

void GameState::addLog(const std::string& detail) {
    addLog(getCurrentPlayerNameOrSystem(), "INFO", detail);
}

void GameState::addLog(const std::string& username, const std::string& actionType, const std::string& detail) {
    logger.log(LogEntry{currentTurn, username, actionType, detail});
}

std::vector<LogEntry> GameState::getLogs() const {
    return logger.getAll();
}

std::vector<LogEntry> GameState::getRecentLogs(int n) const {
    return logger.getRecent(n);
}

Logger& GameState::getLogger() {
    return logger;
}

const Logger& GameState::getLogger() const {
    return logger;
}

Dice& GameState::getDice() {
    return dice;
}

const Dice& GameState::getDice() const {
    return dice;
}

Board& GameState::getBoard() {
    if (!board) {
        throw std::runtime_error("Board belum diinisialisasi.");
    }
    return *board;
}

const Board& GameState::getBoard() const {
    if (!board) {
        throw std::runtime_error("Board belum diinisialisasi.");
    }
    return *board;
}

void GameState::setBoard(const std::shared_ptr<Board>& newBoard) {
    if (!newBoard) {
        throw std::invalid_argument("Board tidak boleh null.");
    }
    board = newBoard;
}

int GameState::getBoardSizeOrDefault(int defaultSize) const {
    if (defaultSize <= 0) {
        throw std::invalid_argument("defaultSize harus lebih dari 0.");
    }

    if (!board || board->getSize() <= 0) {
        return defaultSize;
    }
    return board->getSize();
}

bool GameState::isGameOver() const {
    return gameOver;
}

void GameState::setGameOver(bool value) {
    gameOver = value;
}

void GameState::clampCurrentPlayerIndex() {
    if (players.empty()) {
        currentPlayerIdx = 0;
        return;
    }
    if (currentPlayerIdx < 0 || currentPlayerIdx >= static_cast<int>(players.size())) {
        currentPlayerIdx = 0;
    }
}

int GameState::countActivePlayers() const {
    int activeCount = 0;
    for (const Player& player : players) {
        if (!player.isBankrupt()) {
            ++activeCount;
        }
    }
    return activeCount;
}

std::string GameState::getCurrentPlayerNameOrSystem() const {
    if (players.empty()) {
        return "SYSTEM";
    }

    int safeIdx = currentPlayerIdx;
    if (safeIdx < 0 || safeIdx >= static_cast<int>(players.size())) {
        safeIdx = 0;
    }
    return players[static_cast<std::size_t>(safeIdx)].getUsername();
}
