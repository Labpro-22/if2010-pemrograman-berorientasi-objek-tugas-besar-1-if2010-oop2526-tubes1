#include "../include/models/GameBoard.hpp"


GameBoard::GameBoard() : currentPlayerIndex(0) {

}

void GameBoard::addTile(Tile* tile) {
    tiles.push_back(tile);
}

Tile* GameBoard::getTileAt(int position) const {
    if (tiles.empty()) return nullptr;
    int index = position % tiles.size();
    return tiles[index];
}


void GameBoard::addPlayer(Player* player) {
    players.push_back(player);
}

Player* GameBoard::getPlayerByUsername(const std::string& username) const {
    for (auto* p : players) {
        if (p->getUsername() == username) {
            return p;
        }
    }
    return nullptr;
}

Player* GameBoard::getCurrentPlayer() const {
    if (players.empty()) return nullptr;
    return players[currentPlayerIndex];
}

const std::vector<Player*>& GameBoard::getPlayers() const {
    return players;
}

const std::vector<Tile*>& GameBoard::getTiles() const {
    return tiles;
}


void GameBoard::nextTurn() {
    if (players.empty()) return;
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}
