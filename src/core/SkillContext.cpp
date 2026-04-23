#include "SkillContext.hpp"

SkillContext::SkillContext(Player& currentPlayer, std::vector<Player>& players, Board& board, Logger& logger)
    : currentPlayer(currentPlayer), players(players), board(board), logger(logger) {}

Player& SkillContext::getCurrentPlayer() const {
    return currentPlayer;
}

std::vector<Player>& SkillContext::getPlayers() const {
    return players;
}

Board& SkillContext::getBoard() const {
    return board;
}

Logger& SkillContext::getLogger() const {
    return logger;
}