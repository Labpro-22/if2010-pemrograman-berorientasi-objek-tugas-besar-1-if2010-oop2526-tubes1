#pragma once
#include <vector>
#include "models/Player/Player.hpp"
#include "utils/Logger.hpp"

class Board;

class SkillContext {
    private:
        Player& currentPlayer;
        std::vector<Player>& players;
        Board& board;
        Logger& logger;

    public:
        SkillContext(Player& currentPlayer, std::vector<Player>& players, Board& board, Logger& logger);
        Player& getCurrentPlayer() const;
        std::vector<Player>& getPlayers() const;
        Board& getBoard() const;
        Logger& getLogger() const;
};
