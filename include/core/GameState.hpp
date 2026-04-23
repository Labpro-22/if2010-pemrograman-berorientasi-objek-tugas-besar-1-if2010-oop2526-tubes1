#pragma once

#include <memory>
#include <string>
#include <vector>
#include "utils/Logger.hpp"
#include "models/Board/Board.hpp"
#include "models/Board/Dice.hpp"
#include "models/Player/Player.hpp"

class GameState {
private:
    std::vector<Player> players;
    int currentPlayerIdx;
    int maxTurn;
    int currentTurn;
    int salary;
    int jailFine;
    int startingCash;
    bool gameOver;
    Logger logger;
    Dice dice;
    std::shared_ptr<Board> board;

    void clampCurrentPlayerIndex();
    int countActivePlayers() const;
    std::string getCurrentPlayerNameOrSystem() const;

public:
    GameState();

    void addPlayer(const Player& player);

    std::vector<Player>& getPlayers();
    const std::vector<Player>& getPlayers() const;

    Player& getCurrentPlayer();
    const Player& getCurrentPlayer() const;

    int getCurrentPlayerIdx() const;
    int getMaxTurn() const;
    int getCurrentTurn() const;
    int getSalary() const;
    int getJailFine() const;
    int getStartingCash() const;

    void setCurrentPlayerIdx(int idx);
    void setMaxTurn(int maxTurn);
    void setCurrentTurn(int turn);
    void setSalary(int amount);
    void setJailFine(int amount);
    void setStartingCash(int amount);
    void nextPlayer();

    std::vector<Player> getActivePlayers() const;
    bool hasSingleActivePlayer() const;


    void addLog(const std::string& detail);
    void addLog(const std::string& username, const std::string& actionType, const std::string& detail);
    std::vector<LogEntry> getLogs() const;
    std::vector<LogEntry> getRecentLogs(int n) const;
    Logger& getLogger();
    const Logger& getLogger() const;
    Dice& getDice();
    const Dice& getDice() const;
    Board& getBoard();
    const Board& getBoard() const;
    void setBoard(const std::shared_ptr<Board>& newBoard);
    int getBoardSizeOrDefault(int defaultSize = Board::DEFAULT_BOARD_SIZE) const;

    bool isGameOver() const;
    void setGameOver(bool value);
};
