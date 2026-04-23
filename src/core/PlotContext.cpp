#include "core/PlotContext.hpp"
#include "models/Player/Player.hpp"
#include "models/Board/Board.hpp"
#include "utils/Logger.hpp"
#include "core/services/AuctionService.hpp"

PlotContext::PlotContext(Player& currentPlayer, std::vector<Player>& players, Board& board, Logger& logger, AuctionService& auctionService)
    : currentPlayer(currentPlayer), players(players), board(board), logger(logger), auctionService(auctionService) {}

Player& PlotContext::getCurrentPlayer() const {
    return currentPlayer;
}

std::vector<Player>& PlotContext::getPlayers() const {
    return players;
}

Board& PlotContext::getBoard() const {
    return board;
}

Logger& PlotContext::getLogger() const {
    return logger;
}

AuctionService& PlotContext::getAuctionService() const {
    return auctionService;
}
