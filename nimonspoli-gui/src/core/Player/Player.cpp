#include "Player.hpp"
#include "../Property/Property.hpp"

// ─────────────────────────────────────────────
//  Konstruktor & destruktor
// ─────────────────────────────────────────────

Player::Player(const string& id, const string& username, int startingBalance)
    :  username(username), balance(startingBalance),
      position(0), currPetak(nullptr),
      status(PlayerStatus::ACTIVE), jailTurns(0),
      cardUsedThisTurn(false), shieldActive(false) {}

Player::~Player() {}

// ─────────────────────────────────────────────
//  Identitas
// ─────────────────────────────────────────────

string Player::getUsername() const { return username; }

// ─────────────────────────────────────────────
//  Keuangan
// ─────────────────────────────────────────────

int Player::getBalance() const { return balance; }
int Player::getMoney()   const { return balance; }

int Player::getWealth() const {
    int total = balance;
    for (Property* p : properties) {
        if (p) total += static_cast<int>(p->getPurchasePrice());
        // Nilai bangunan ditambahkan oleh StreetProperty::calculateSellPrice()
        // jika sudah diimplementasi — saat ini gunakan purchasePrice sebagai baseline
    }
    return total;
}

Player& Player::operator+=(int amount) { balance += amount; return *this; }
Player& Player::operator-=(int amount) { balance -= amount; return *this; }
bool    Player::canAfford(int amount)  const { return balance >= amount; }
bool    Player::operator>(const Player& other) const { return balance > other.balance; }
bool    Player::operator<(const Player& other) const { return balance < other.balance; }

// ─────────────────────────────────────────────
//  Posisi
// ─────────────────────────────────────────────

int   Player::getPosition()  const { return position; }
Tile* Player::getCurrPetak() const { return currPetak; }

void Player::setPosition(int tileIndex) {
    position = tileIndex;
}

void Player::setCurrPetak(Tile* tile) {
    currPetak = tile;
}

void Player::move(int steps) {
    // Implementasi menunggu Board selesai.
    // GameMaster::movePlayer() yang memanggil setCurrPetak() dan setPosition()
    // setelah menghitung targetIdx = (position + steps) % board.getSize()
    (void)steps;
}

// ─────────────────────────────────────────────
//  Status
// ─────────────────────────────────────────────

PlayerStatus Player::getStatus() const { return status; }

void Player::setStatus(PlayerStatus s) { status = s; }

string Player::getStatusString() const {
    switch (status) {
        case PlayerStatus::ACTIVE:   return "ACTIVE";
        case PlayerStatus::JAILED:   return "JAILED";
        case PlayerStatus::BANKRUPT: return "BANKRUPT";
        default:                     return "UNKNOWN";
    }
}

// ─────────────────────────────────────────────
//  Penjara
// ─────────────────────────────────────────────

int  Player::getJailTurns()       const { return jailTurns; }
void Player::setJailTurns(int t)        { jailTurns = t; }
bool Player::isInJail()           const { return status == PlayerStatus::JAILED; }
void Player::incrementJailTurns()       { jailTurns++; }

void Player::goToJail() {
    status    = PlayerStatus::JAILED;
    jailTurns = 0;
}

void Player::releaseFromJail() {
    status    = PlayerStatus::ACTIVE;
    jailTurns = 0;
}

// ─────────────────────────────────────────────
//  Properti
// ─────────────────────────────────────────────

void Player::addProperty(Property* prop) {
    if (prop) properties.push_back(prop);
}

void Player::removeProperty(Property* prop) {
    auto it = find(properties.begin(), properties.end(), prop);
    if (it != properties.end()) properties.erase(it);
}

Property* Player::getPropertyAt(int pos) const {
    if (pos < 0 || pos >= static_cast<int>(properties.size())) return nullptr;
    return properties[pos];
}

const vector<Property*>& Player::getProperties()   const { return properties; }
int Player::getPropertyCount()                      const { return static_cast<int>(properties.size()); }
int Player::getPropertyNum()                        const { return getPropertyCount(); }

// ─────────────────────────────────────────────
//  Kartu Kemampuan
// ─────────────────────────────────────────────

bool Player::addSkillCard(SkillCard* card) {
    if (!card) return false;
    if (static_cast<int>(skillCards.size()) >= 3) return false; // tangan penuh
    skillCards.push_back(card);
    return true;
}

void Player::discardSkillCard(int index) {
    if (index < 0 || index >= static_cast<int>(skillCards.size())) return;
    skillCards.erase(skillCards.begin() + index);
}

SkillCard* Player::getSkillCardAt(int index) const {
    if (index < 0 || index >= static_cast<int>(skillCards.size())) return nullptr;
    return skillCards[index];
}

const vector<SkillCard*>& Player::getHand()    const { return skillCards; }
int Player::getHandSize()                      const { return static_cast<int>(skillCards.size()); }
int Player::getCardNum()                       const { return getHandSize(); }
void Player::setCardUsedThisTurn(bool used)          { cardUsedThisTurn = used; }
bool Player::hasUsedCardThisTurn()             const { return cardUsedThisTurn; }

// ─────────────────────────────────────────────
//  Shield
// ─────────────────────────────────────────────

void Player::activateShield()   { shieldActive = true; }
void Player::deactivateShield() { shieldActive = false; }
bool Player::isShielded()  const { return shieldActive; }

// ─────────────────────────────────────────────
//  Turn lifecycle
// ─────────────────────────────────────────────

void Player::onTurnStart() {
    cardUsedThisTurn = false;
    shieldActive     = false;
}