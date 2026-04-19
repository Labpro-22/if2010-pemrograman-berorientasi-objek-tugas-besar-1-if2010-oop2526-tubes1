#include "../../include/core/GameEngine.hpp"
#include "../../include/core/CardManager.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/utils/GameException.hpp"

// ─── Header dari Orang 1 & 2 (uncomment saat sudah tersedia) ─────────────────
// #include "../../include/models/Player.hpp"
// #include "../../include/models/Board.hpp"
// #include "../../include/core/Command.hpp"

// ─── Header dari Orang 4 & 5 (uncomment saat sudah tersedia) ─────────────────
// #include "../../include/models/Bank.hpp"
// #include "../../include/core/AuctionManager.hpp"
// #include "../../include/core/BankruptcyManager.hpp"
// #include "../../include/core/PropertyManager.hpp"
// #include "../../include/core/CardManager.hpp"
// #include "../../include/core/EffectManager.hpp"
// #include "../../include/core/TransactionLogger.hpp"
// #include "../../include/core/SaveLoadManager.hpp"

#include <iostream>
#include <algorithm>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

GameEngine::GameEngine()
    : board(nullptr),
      gameOver(false),
      maxTurn(0),
      goSalary(200),
      jailFine(50),
      dice(6),
      bank(nullptr),
      auctionManager(nullptr),
      bankruptcyManager(nullptr),
      propertyManager(nullptr),
      cardManager(nullptr),
      effectManager(nullptr),
      logger(nullptr) {}

GameEngine::~GameEngine() {
    delete board;
    board = nullptr;

    for (Player* player : players) {
        delete player;
    }
    players.clear();
}

// ─────────────────────────────────────────────────────────────────────────────
// Dependency injection
// ─────────────────────────────────────────────────────────────────────────────

void GameEngine::setBank(Bank* b)                          { bank               = b;  }
void GameEngine::setAuctionManager(AuctionManager* am)     { auctionManager     = am; }
void GameEngine::setBankruptcyManager(BankruptcyManager* bm){ bankruptcyManager = bm; }
void GameEngine::setPropertyManager(PropertyManager* pm)   { propertyManager    = pm; }
void GameEngine::setCardManager(CardManager* cm)           { cardManager        = cm; }
void GameEngine::setEffectManager(EffectManager* em)       { effectManager      = em; }
void GameEngine::setTransactionLogger(TransactionLogger* tl){ logger            = tl; }

// ─────────────────────────────────────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────────────────────────────────────

void GameEngine::startNewGame(int nPlayers, std::vector<std::string> names) {
    for (Player* player : players) {
        delete player;
    }
    players.clear();

    if (!board) {
        board = new Board();
    }

    for (int i = 0; i < nPlayers && i < static_cast<int>(names.size()); ++i) {
        players.push_back(new Player(names[i], 1000));
    }

    turnManager.initializeOrder(static_cast<int>(players.size()));

    gameOver = false;

    if (cardManager) {
        cardManager->initializeDecks();
    }
}

void GameEngine::loadGame(const std::string& filename) {
    // TODO: Panggil SaveLoadManager::load(filename) saat Orang 4 selesai
    // TODO: Pulihkan board, players, turnManager, dan state manager lain
    (void)filename;
    throw GameException("loadGame: belum diimplementasikan (menunggu SaveLoadManager)");
}

void GameEngine::run() {
    // Loop utama — berjalan hingga gameOver di-set oleh checkWinCondition/endGame
    while (!gameOver) {
        executeTurn();
    }
}

void GameEngine::processCommand(const Command& cmd) {
    // TODO: Dispatch ke handler berdasarkan cmd.type
    // Contoh switch akan ditambahkan saat Command & enum CommandType sudah ada (Orang 5)
    (void)cmd;
    throw GameException("processCommand: belum diimplementasikan (menunggu Command/CommandType)");
}

void GameEngine::executeTurn() {
    if (players.empty()) {
        throw GameException("executeTurn: tidak ada pemain aktif");
    }

    Player& current = getCurrentPlayer();
    if (cardManager) {
        cardManager->drawSkillCard(current);
    }

    checkWinCondition();
    turnManager.nextPlayer(buildBankruptFlags());
}

void GameEngine::moveCurrentPlayer(int steps) {
    if (!board) {
        throw GameException("moveCurrentPlayer: board belum diinisialisasi");
    }

    Player& player = getCurrentPlayer();
    const int oldPos = player.getPosition();
    player.move(steps, board->size());

    if (player.getPosition() < oldPos) {
        awardPassGoSalary(player);
    }

    Tile& landing = board->getTileByIndex(player.getPosition());
    handleLanding(player, landing);
}

void GameEngine::handleLanding(Player& p, Tile& t) {
    t.onLand(p, *this);
}

void GameEngine::checkWinCondition() {
    int activeCount = 0;
    for (Player* player : players) {
        if (player && !player->isBankrupt()) {
            activeCount++;
        }
    }

    if (activeCount <= 1) {
        endGame();
        return;
    }

    if (maxTurn > 0 && getCurrentTurn() >= maxTurn) {
        endGame();
    }
}

void GameEngine::endGame() {
    gameOver = true;
    // TODO: Tentukan pemenang berdasarkan getTotalWealth() (Orang 1)
    // TODO: Tampilkan hasil akhir via GameUI (Orang 5)
}

// ─────────────────────────────────────────────────────────────────────────────
// Akses state
// ─────────────────────────────────────────────────────────────────────────────

Player& GameEngine::getCurrentPlayer() {
    int idx = turnManager.getCurrentPlayerIndex();
    if (idx < 0 || idx >= static_cast<int>(players.size())) {
        throw std::out_of_range("getCurrentPlayer: index pemain di luar batas");
    }
    return *players[idx];
}

Player* GameEngine::getPlayerByName(const std::string& name) {
    for (auto* p : players) {
        if (p && p->getUsername() == name) {
            return p;
        }
    }
    return nullptr;
}

std::vector<Player*> GameEngine::getActivePlayers() const {
    std::vector<Player*> active;
    for (auto* p : players) {
        if (p && !p->isBankrupt()) {
            active.push_back(p);
        }
    }
    return active;
}

Board& GameEngine::getBoard() {
    if (!board) throw GameException("Board belum diinisialisasi");
    return *board;
}

Dice&         GameEngine::getDice()             { return dice;             }
TurnManager&  GameEngine::getTurnManager()      { return turnManager;      }

Bank& GameEngine::getBank() {
    if (!bank) throw GameException("Bank belum di-inject");
    return *bank;
}
AuctionManager& GameEngine::getAuctionManager() {
    if (!auctionManager) throw GameException("AuctionManager belum di-inject");
    return *auctionManager;
}
BankruptcyManager& GameEngine::getBankruptcyManager() {
    if (!bankruptcyManager) throw GameException("BankruptcyManager belum di-inject");
    return *bankruptcyManager;
}
PropertyManager& GameEngine::getPropertyManager() {
    if (!propertyManager) throw GameException("PropertyManager belum di-inject");
    return *propertyManager;
}
CardManager& GameEngine::getCardManager() {
    if (!cardManager) throw GameException("CardManager belum di-inject");
    return *cardManager;
}
EffectManager& GameEngine::getEffectManager() {
    if (!effectManager) throw GameException("EffectManager belum di-inject");
    return *effectManager;
}
TransactionLogger& GameEngine::getLogger() {
    if (!logger) throw GameException("TransactionLogger belum di-inject");
    return *logger;
}

bool GameEngine::isGameOver()    const { return gameOver;                    }
int  GameEngine::getMaxTurn()    const { return maxTurn;                     }
int  GameEngine::getGoSalary()   const { return goSalary;                    }
int  GameEngine::getJailFine()   const { return jailFine;                    }
int  GameEngine::getCurrentTurn() const{ return turnManager.getTurnNumber(); }

// ─────────────────────────────────────────────────────────────────────────────
// Helper private
// ─────────────────────────────────────────────────────────────────────────────

void GameEngine::initBoard() {
    if (!board) {
        board = new Board();
    }
}

void GameEngine::handleJailTurn(Player& p) {
    // TODO: Implementasikan logika penjara:
    //   - jailTurns < 3: tawarkan pilihan lempar double atau bayar denda
    //   - jailTurns == 3: paksa bayar denda lalu lempar dadu normal
    (void)p;
}

void GameEngine::awardPassGoSalary(Player& p) {
    p.addMoney(goSalary);
}

std::vector<bool> GameEngine::buildBankruptFlags() const {
    std::vector<bool> flags(players.size(), false);
    for (int i = 0; i < static_cast<int>(players.size()); ++i) {
        if (players[i]) {
            flags[i] = players[i]->isBankrupt();
        }
    }
    return flags;
}