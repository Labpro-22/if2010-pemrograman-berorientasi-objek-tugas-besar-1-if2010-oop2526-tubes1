#include "../../include/core/GameController.hpp"
#include "../../include/models/GameBoard.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/models/Property.hpp"
#include <iostream>

using namespace std;

GameController::GameController() {
    gameBoard = new GameBoard();
}

GameController::~GameController() {
    if (gameBoard != nullptr) {
        delete gameBoard;
    }
}

void GameController::startGame() {
    initializeGame();
    cout << "[GameController] Game berhasil dimulai!\n";
}

void GameController::initializeGame() {
    // TODO: Initialize 40-tile board with properties
    // TODO: Create and initialize players
    cout << "[GameController] Board dan players sudah diinisialisasi\n";
}

void GameController::addPlayer(const std::string& username, int startingMoney) {
    if (gameBoard != nullptr) {
        std::shared_ptr<Player> newPlayer = std::make_shared<Player>(username, startingMoney);
        gameBoard->addPlayer(newPlayer);
        cout << "[GameController] Player " << username << " ditambahkan dengan uang " << startingMoney << endl;
    }
}

void GameController::processTurn(Player& player, int diceResult) {
    (void)player;
    (void)diceResult;

    cout << "[GameController] processTurn\n";

    // nanti:
    // movementHandler->movePlayer(...)
}

void GameController::processLanding(Player& player, Tile* tile) {
    (void)player;

    if (tile == nullptr) return;

    cout << "[GameController] processLanding di " << tile->getName() << endl;

    // nanti:
    // tile->onLand(player)
}

void GameController::processPurchase(Player& player, Property* property) {
    (void)player;

    if (property == nullptr) return;

    cout << "[GameController] processPurchase " << property->getName() << endl;

    // nanti:
    // bank->sellProperty(...)
}

void GameController::processAuction(Property* property) {
    if (property == nullptr) return;

    cout << "[GameController] processAuction\n";

    // nanti:
    // auction->startAuction()
}

void GameController::processBankruptcy(Player& player) {
    cout << "[GameController] processBankruptcy: " << player.getUsername() << endl;

    // nanti:
    // bank->declareBankruptcy(...)
}

bool GameController::checkGameEnd() {
    cout << "[GameController] checkGameEnd\n";

    // nanti:
    // return winConditionChecker->isGameOver(...)
    return false;
}

Player* GameController::getWinner() {
    cout << "[GameController] getWinner\n";
    return nullptr; // TODO: implement win condition
}

// ==========================
// GETTER UNTUK UI (WAJIB)
// ==========================

GameBoard& GameController::getBoard() {
    return *gameBoard;
}

const std::vector<std::shared_ptr<Player>>& GameController::getPlayers() const {
    return gameBoard->getPlayers();
}

std::shared_ptr<Player> GameController::getPlayerByUsername(const std::string& username) {
    return gameBoard->getPlayerByUsername(username);
}