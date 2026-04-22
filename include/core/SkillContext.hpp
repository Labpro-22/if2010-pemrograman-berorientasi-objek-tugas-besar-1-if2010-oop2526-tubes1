#pragma once
#include <vector>
#include "models/Player/Player.hpp"
#include "utils/Logger.hpp"

class Board;

struct SkillContext {
    Player& currentPlayer;
    std::vector<Player>& players;
    Board& board;
    Logger& logger;
};
