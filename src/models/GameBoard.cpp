#include "../include/models/GameBoard.hpp"
#include "../include/models/Player.hpp"


GameBoard::GameBoard()
    : currentPlayerIndex(0),
      currentTurnNumber(1),
      maxTurn(0),
      skillDeck(nullptr) {

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

int GameBoard::getCurrentTurnNumber() const {
    return currentTurnNumber;
}

void GameBoard::setCurrentTurnNumber(int t) {
    currentTurnNumber = t;
}

int GameBoard::getMaxTurn() const {
    return maxTurn;
}

void GameBoard::setMaxTurn(int m) {
    maxTurn = m;
}

CardDeck<SkillCard>* GameBoard::getSkillDeck() const {
    return skillDeck;
}

void GameBoard::setSkillDeck(CardDeck<SkillCard>* d) {
    skillDeck = d;
}

void GameBoard::setCurrentPlayerByUsername(const std::string& username) {
    for (size_t i = 0; i < players.size(); ++i) {
        if (players[i] != nullptr && players[i]->getUsername() == username) {
            currentPlayerIndex = static_cast<int>(i);
            return;
        }
    }
}

void GameBoard::setTurnOrder(const std::vector<std::string>& order) {
    std::vector<Player*> reordered;
    reordered.reserve(players.size());
    for (size_t i = 0; i < order.size(); ++i) {
        for (size_t j = 0; j < players.size(); ++j) {
            if (players[j] != nullptr && players[j]->getUsername() == order[i]) {
                reordered.push_back(players[j]);
                break;
            }
        }
    }
    for (size_t j = 0; j < players.size(); ++j) {
        if (players[j] == nullptr) continue;
        bool already = false;
        for (size_t k = 0; k < reordered.size(); ++k) {
            if (reordered[k] == players[j]) { already = true; break; }
        }
        if (!already) reordered.push_back(players[j]);
    }
    if (!reordered.empty()) {
        players = reordered;
        currentPlayerIndex = 0;
    }
}
