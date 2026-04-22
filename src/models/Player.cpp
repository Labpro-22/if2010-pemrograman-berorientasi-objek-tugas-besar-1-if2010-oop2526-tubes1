#include "../../include/models/Player.hpp"
#include "../../include/utils/SkillCard.hpp"

Player::Player(string name, int startMoney) {
    username = name;
    money = startMoney;
    position = 0;
    status = ACTIVE;

    jailTurnsRemaining = 0;
    consecutiveDoubles = 0;

    hasUsedCardThisTurn = false;
    hasRolledThisTurn = false;

    shieldActive = false;
    discountPercent = 0;
}

Player::~Player() {
    for (SkillCard* card : hand) {
        delete card;
    }
    hand.clear();
}

string Player::getUsername() { return username; }
int Player::getMoney() { return money; }
int Player::getPosition() { return position; }
PlayerStatus Player::getStatus() { return status; }

vector<SkillCard*>& Player::getHand() { return hand; }
bool Player::hasRolled() { return hasRolledThisTurn; }
bool Player::hasUsedCard() { return hasUsedCardThisTurn; }
bool Player::isShieldActive() { return shieldActive; }
int Player::getDiscountPercent() { return discountPercent; }

void Player::setPosition(int pos) { position = pos; }
void Player::setStatus(PlayerStatus s) { status = s; }

Player& Player::operator+=(int amt) {
    money += amt;
    return *this;
}

Player& Player::operator-=(int amt) {
    if (shieldActive) {
        shieldActive = false;
        return *this;
    }
    money -= amt;
    return *this;
}

void Player::incrementConsecutiveDoubles() { consecutiveDoubles++; }
void Player::resetConsecutiveDoubles() { consecutiveDoubles = 0; }
int Player::getConsecutiveDoubles() { return consecutiveDoubles; }

void Player::incrementJailTurns() { jailTurnsRemaining++; }
void Player::resetJailTurns() { jailTurnsRemaining = 0; }
int Player::getJailTurnsRemaining() { return jailTurnsRemaining; }

void Player::markRolled() { hasRolledThisTurn = true; }
void Player::markCardUsed() { hasUsedCardThisTurn = true; }

void Player::resetTurnFlags() {
    hasRolledThisTurn = false;
    hasUsedCardThisTurn = false;
    shieldActive = false;
    discountPercent = 0;
}

void Player::receiveCard(SkillCard* c) {
    hand.push_back(c);
}

SkillCard* Player::removeCard(int idx) {
    if (idx < 0 || idx >= static_cast<int>(hand.size())) {
        return nullptr;
    }
    SkillCard* c = hand[idx];
    hand.erase(hand.begin() + idx);
    return c;
}

void Player::activateShield() { shieldActive = true; }
void Player::clearShield() { shieldActive = false; }

void Player::setDiscount(int d) { discountPercent = d; }
void Player::clearDiscount() { discountPercent = 0; }
