#include "../../include/core/ConfigLoader.hpp"
#include "../../include/core/Exceptions.hpp"
#include "../../include/core/GameEngine.hpp"
#include "../../include/core/Player.hpp"
#include "../../include/core/TransactionLogger.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Card.hpp"
#include "../../include/models/PropertyTile.hpp"
#include "../../include/models/Tile.hpp"
#include <algorithm>
#include <iostream>
#include <random>

using namespace std;

// Konstruktor menginisialisasi state awal permainan
GameEngine::GameEngine(Board *b, TransactionLogger *l)
    : board(b), saveLoadManager(nullptr), logger(l), skillDeck(nullptr), currentTurnIdx(0), roundCount(0), diceRolled(false), turnEnded(false) {
}

// Destruktor bertanggung jawab menghapus semua pointer Player yang di-alokasi
GameEngine::~GameEngine() {
    for (Player *p : players) {
        delete p;
    }
    players.clear();
}

void GameEngine::startNewGame(const std::vector<std::string>& playerNames) {
    // 1. Reset state jika sebelumnya sudah ada permainan
    for (Player *p : players)
        delete p;
    players.clear();
    roundCount = 0;
    currentTurnIdx = 0;
    diceRolled = false;
    turnEnded = false;

    int initialMoney = ConfigLoader::getInstance()->getInitialMoney();

    for (const string& name : playerNames) {
        players.push_back(new Player(name, initialMoney));
    }

    if (logger)
        logger->logEvent(0, "SYSTEM", LogActionType::LOAD, "Game Started");
}

void GameEngine::loadGame(const std::string& filePath) {
    // TODO: Implementasi load game menggunakan saveLoadManager
}

void GameEngine::saveGame(const std::string& filePath) {
    // TODO: Implementasi save game menggunakan saveLoadManager
}

void GameEngine::rollDice(int d1, int d2) {
    if (diceRolled) {
        throw InvalidCommandException("Dadu sudah dikocok pada giliran ini");
    }

    int finalD1 = d1;
    int finalD2 = d2;

    if (d1 == 0 && d2 == 0) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 6);
        finalD1 = dis(gen);
        finalD2 = dis(gen);
    } else if (d1 < 1 || d1 > 6 || d2 < 1 || d2 > 6) {
        throw InvalidCommandException("Nilai dadu tidak valid");
    }

    int diceTotal = finalD1 + finalD2;
    Player *activePlayer = players[currentTurnIdx];

    // Gerakkan pemain berdasarkan total dadu
    activePlayer->move(diceTotal);
    diceRolled = true;

    // Picu efek petak tempat mendarat
    Tile *landedTile = board->getTileAt(activePlayer->getPosition());
    if (landedTile) {
        landedTile->onLanded(*activePlayer);
    }
}

void GameEngine::executeTileAction() {
    if (!diceRolled) {
        throw InvalidCommandException("Lempar dadu terlebih dahulu");
    }
    Player *activePlayer = players[currentTurnIdx];
    Tile *landedTile = board->getTileAt(activePlayer->getPosition());
    if (landedTile) {
        landedTile->onLanded(*activePlayer);
    }
}

void GameEngine::endTurn() {
    turnEnded = true;
    nextTurn();
}

void GameEngine::nextTurn() {
    // Cari pemain berikutnya yang tidak bangkrut
    if (players.empty()) return;

    int nextIdx = (currentTurnIdx + 1) % players.size();
    while (players[nextIdx]->getStatus() == PlayerStatus::BANKRUPT) {
        nextIdx = (nextIdx + 1) % players.size();
        if (nextIdx == currentTurnIdx)
            break; // Semua bangkrut? Harusnya sudah selesai
    }

    if (nextIdx < currentTurnIdx) {
        roundCount++; // Melewati pemain pertama, tambah putaran
    }

    currentTurnIdx = nextIdx;
    diceRolled = false;
    turnEnded = false;
    checkWinCondition();
}

void GameEngine::checkWinCondition() {
    int activePlayers = 0;
    Player *potentialWinner = nullptr;

    for (Player *p : players) {
        if (p->getStatus() != PlayerStatus::BANKRUPT) {
            activePlayers++;
            potentialWinner = p;
        }
    }

    // Kondisi Menang 1: Hanya tersisa satu pemain aktif
    if (activePlayers == 1) {
        // Trigger UI Winner melalui controller/main
        return;
    }

    // Kondisi Menang 2: Batas ronde tercapai
    int maxTurn = ConfigLoader::getInstance()->getMaxTurn();
    if (roundCount >= maxTurn) {
        // Cari kekayaan tertinggi (Uang + Harga Properti)
    }
}

Player* GameEngine::getCurrentPlayer() const {
    if (players.empty() || currentTurnIdx < 0 || currentTurnIdx >= (int)players.size()) return nullptr;
    return players[currentTurnIdx];
}
