#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "core/Types.h"

namespace Nimonspoli {

class Property;
class SkillCard;

class Player {
public:
    static constexpr int MAX_HAND_SIZE = 3;

    explicit Player(const std::string& username, int startBalance)
        : username_(username), balance_(startBalance) {}

    // ── Operator overloads ───────────────────────────────────────────────────
    // Add/subtract money (positive = receive, negative = pay)
    Player& operator+=(int amount) {
        balance_ += amount;
        return *this;
    }
    Player& operator-=(int amount) {
        balance_ -= amount;
        return *this;
    }

    // Compare total net worth for end-game ranking
    bool operator<(const Player& other) const {
        return this->netWorth() < other.netWorth();
    }
    bool operator>(const Player& other) const { return other < *this; }

    // ── Identity ─────────────────────────────────────────────────────────────
    const std::string& username() const { return username_; }

    // ── Balance ──────────────────────────────────────────────────────────────
    int  balance()     const { return balance_; }
    bool canAfford(int amount) const { return balance_ >= amount; }

    // ── Position ─────────────────────────────────────────────────────────────
    int  position()    const { return position_; }
    void setPosition(int pos) { position_ = pos; }

    // ── Status ───────────────────────────────────────────────────────────────
    PlayerStatus status()      const { return status_; }
    void setStatus(PlayerStatus s)   { status_ = s; }
    bool isActive()   const { return status_ == PlayerStatus::ACTIVE; }
    bool isJailed()   const { return status_ == PlayerStatus::JAILED; }
    bool isBankrupt() const { return status_ == PlayerStatus::BANKRUPT; }

    // ── Jail ─────────────────────────────────────────────────────────────────
    int  jailTurns()  const { return jailTurns_; }
    void setJailTurns(int n)    { jailTurns_ = n; }
    void incrementJailTurns()   { ++jailTurns_; }
    void resetJailTurns()       { jailTurns_ = 0; }

    // ── Properties ───────────────────────────────────────────────────────────
    void addProperty(Property* p)    { properties_.push_back(p); }
    void removeProperty(Property* p) {
        auto it = std::find(properties_.begin(), properties_.end(), p);
        if (it != properties_.end()) properties_.erase(it);
    }
    const std::vector<Property*>& properties() const { return properties_; }

    // ── Skill cards (hand) ────────────────────────────────────────────────────
    bool handFull()    const { return (int)hand_.size() >= MAX_HAND_SIZE; }
    int  handSize()    const { return (int)hand_.size(); }

    // Returns true if successfully added, false if hand was full (caller must drop)
    void addToHand(SkillCard* card) { hand_.push_back(card); }
    void removeFromHand(int index) {
        if (index < 0 || index >= (int)hand_.size())
            throw std::out_of_range("Invalid hand index.");
        hand_.erase(hand_.begin() + index);
    }
    const std::vector<SkillCard*>& hand() const { return hand_; }

    // ── Shield / Discount state (set by SkillCards) ──────────────────────────
    bool  isShielded()       const { return shielded_; }
    void  setShielded(bool v)      { shielded_ = v; }

    bool  hasDiscount()      const { return discountPct_ > 0; }
    int   discountPct()      const { return discountPct_; }
    void  setDiscountPct(int pct)  { discountPct_ = pct; }
    void  clearDiscount()          { discountPct_ = 0; }

    // ── Turn state ────────────────────────────────────────────────────────────
    bool  usedCardThisTurn()   const { return usedCardThisTurn_; }
    void  setUsedCard(bool v)        { usedCardThisTurn_ = v; }
    bool  hasRolled()          const { return hasRolled_; }
    void  setHasRolled(bool v)       { hasRolled_ = v; }

    void resetTurnState() {
        usedCardThisTurn_ = false;
        hasRolled_        = false;
        shielded_         = false;
        clearDiscount();
    }

    // ── Net worth (for ranking and PPH calculation) ───────────────────────────
    // cash + buy price of all properties + buy price of all buildings
    int netWorth() const;

    // Maximum potential cash from liquidating all assets (for bankruptcy check)
    int maxLiquidation() const;

private:
    std::string          username_;
    int                  balance_          = 0;
    int                  position_         = 0;   // tile index (0-based)
    PlayerStatus         status_           = PlayerStatus::ACTIVE;
    int                  jailTurns_        = 0;

    std::vector<Property*>  properties_;
    std::vector<SkillCard*> hand_;           // non-owning; CardDeck owns cards

    bool  shielded_          = false;
    int   discountPct_       = 0;
    bool  usedCardThisTurn_  = false;
    bool  hasRolled_         = false;
};

} // namespace Nimonspoli