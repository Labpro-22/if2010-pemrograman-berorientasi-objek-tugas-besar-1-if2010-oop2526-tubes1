#include "models/Player/Player.hpp"

#include <stdexcept>
#include <utility>
#include "core/GameException.hpp"

Player::Player()
    : username(""), cash(0), position(0), status(PlayerStatus::ACTIVE), jailTurns(0), usedSkillThisTurn(false) {}

Player::Player(std::string username, int startingCash, int startPosition)
    : username(std::move(username)),
      cash(startingCash),
      position(startPosition),
      status(PlayerStatus::ACTIVE),
      jailTurns(0),
      usedSkillThisTurn(false) {
    if (startingCash < 0) {
        throw std::invalid_argument("startingCash tidak boleh negatif.");
    }
}

const std::string& Player::getUsername() const {
    return username;
}

int Player::getCash() const {
    return cash;
}

int Player::getPosition() const {
    return position;
}

PlayerStatus Player::getStatus() const {
    return status;
}

int Player::getJailTurns() const {
    return jailTurns;
}

bool Player::hasUsedSkillThisTurn() const {
    return usedSkillThisTurn;
}

void Player::move(int steps, int boardSize) {
    if (boardSize <= 0) {
        throw std::invalid_argument("boardSize harus lebih dari 0.");
    }

    int next = (position + steps) % boardSize;
    if (next < 0) {
        next += boardSize;
    }
    position = next;
}

void Player::moveTo(int index, int boardSize) {
    if (boardSize <= 0) {
        throw std::invalid_argument("boardSize harus lebih dari 0.");
    }
    if (index < 0 || index >= boardSize) {
        throw std::out_of_range("Posisi tujuan di luar board.");
    }

    position = index;
}

void Player::pay(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Nilai pembayaran tidak boleh negatif.");
    }
    if (cash < amount) {
        throw InsufficientFundException();
    }
    cash -= amount;
}

void Player::receive(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Nilai penerimaan tidak boleh negatif.");
    }
    cash += amount;
}

void Player::setStatus(PlayerStatus newStatus) {
    status = newStatus;
    if (newStatus == PlayerStatus::BANKRUPT) {
        cash = 0;
        jailTurns = 0;
    }
}

void Player::setJailTurns(int turns) {
    if (turns < 0) {
        throw std::invalid_argument("Jumlah giliran penjara tidak boleh negatif.");
    }
    jailTurns = turns;
}

void Player::decrementJailTurns() {
    if (jailTurns > 0) {
        --jailTurns;
    }
}

void Player::setUsedSkillThisTurn(bool used) {
    usedSkillThisTurn = used;
}

void Player::resetTurnFlags() {
    usedSkillThisTurn = false;
}

bool Player::isBankrupt() const {
    return status == PlayerStatus::BANKRUPT;
}
