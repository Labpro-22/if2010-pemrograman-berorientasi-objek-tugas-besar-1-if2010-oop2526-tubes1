#include "../include/models/GameBoard.hpp"


GameBoard::GameBoard() : currentPlayerIndex(0) {

}

void GameBoard::addTile(std::unique_ptr<Tile> tile) {
    tiles.push_back(std::move(tile));
}

Tile* GameBoard::getTileAt(int position) const {
    if (tiles.empty()) return nullptr;
    int index = position % tiles.size();
    return tiles[index].get();
}


void GameBoard::addPlayer(std::shared_ptr<Player> player) {
    players.push_back(player);
}

std::shared_ptr<Player> GameBoard::getPlayerByUsername(const std::string& username) const {
    for (const auto& p : players) {
        if (p->getUsername() == username) {
            return p;
        }
    }
    return nullptr;
}

std::shared_ptr<Player> GameBoard::getCurrentPlayer() const {
    if (players.empty()) return nullptr;
    return players[currentPlayerIndex];
}

const std::vector<std::shared_ptr<Player>>& GameBoard::getPlayers() const {
    return players;
}

const std::vector<std::unique_ptr<Tile>>& GameBoard::getTiles() const {
    return tiles;
}


void GameBoard::nextTurn() {
    if (players.empty()) return;
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}
