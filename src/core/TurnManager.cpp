#include "../../include/core/TurnManager.hpp"
#include "../../include/models/Player.hpp"

TurnManager::TurnManager(vector<Player*> p, int maxT) {
    players = p;
    turnOrder.clear();
    for (Player* player : players) {
        if (player != nullptr) {
            turnOrder.push_back(player->getUsername());
        }
    }
    currentIndex = 0;
    currentTurnNumber = 1;
    maxTurn = maxT;
}

Player* TurnManager::getCurrentPlayer() {
    if (players.empty()) return nullptr;

    // Cek dulu player sekarang
    if (players[currentIndex] != nullptr &&
        players[currentIndex]->getStatus() != BANKRUPT) {
        return players[currentIndex];
    }

    // Cari player aktif berikutnya
    int checked = 0;
    while (checked < (int)players.size()) {
        if (players[currentIndex] != nullptr &&
            players[currentIndex]->getStatus() != BANKRUPT) {
            return players[currentIndex];
        }
        currentIndex = (currentIndex + 1) % players.size();
        checked++;
    }

    return nullptr; // semua bankrupt
}

void TurnManager::advanceToNextPlayer() {
    if (players.empty()) return;

    int startIndex = currentIndex;
    currentIndex = (currentIndex + 1) % (int)players.size();

    // Cek apakah sudah satu putaran penuh (wrap ke index 0)
    if (currentIndex == 0) currentTurnNumber++;

    int visited = 0;
    while (visited < (int)players.size() &&
           (players[currentIndex] == nullptr ||
            players[currentIndex]->getStatus() == BANKRUPT)) {
        currentIndex = (currentIndex + 1) % (int)players.size();
        // Jangan increment di sini — hanya geser index
        visited++;
    }
}

int TurnManager::getCurrentTurnNumber() { return currentTurnNumber; }
int TurnManager::getMaxTurn() { return maxTurn; }

bool TurnManager::isMaxTurnReached() {
    return currentTurnNumber > maxTurn;
}

vector<string> TurnManager::getTurnOrder() {
    if (turnOrder.empty()) {
        vector<string> order;
        for (Player* player : players) {
            if (player != nullptr) {
                order.push_back(player->getUsername());
            }
        }
        return order;
    }

    return turnOrder;
}

void TurnManager::setTurnOrder(vector<string> order) {
    turnOrder = order;

    vector<Player*> reordered;
    reordered.reserve(players.size());

    for (const string& username : turnOrder) {
        for (Player* player : players) {
            if (player != nullptr && player->getUsername() == username) {
                reordered.push_back(player);
                break;
            }
        }
    }

    for (Player* player : players) {
        bool alreadyAdded = false;
        for (Player* reorderedPlayer : reordered) {
            if (reorderedPlayer == player) {
                alreadyAdded = true;
                break;
            }
        }

        if (!alreadyAdded && player != nullptr) {
            reordered.push_back(player);
        }
    }

    if (!reordered.empty()) {
        players = reordered;
        currentIndex = 0;
    }
}

void TurnManager::resetTurnFlags(Player* p) {
    if (p == nullptr) {
        return;
    }

    p->resetTurnFlags();
}
