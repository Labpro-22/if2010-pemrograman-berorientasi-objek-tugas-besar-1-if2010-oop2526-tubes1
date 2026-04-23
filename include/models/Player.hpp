#pragma once
#include <vector>
#include <memory>

class Game;
class PropertyTile;
class AbilityCard;
class Account;

enum class PlayerStatus {
    ACTIVE,
    JAILED,
    BANKRUPT
};

class Player
{
private:
    Account* account;
    int money;
    int position;
    PlayerStatus status;
    std::vector<PropertyTile*> ownedProperties;
    std::vector<std::unique_ptr<AbilityCard>> handCards;
    int jailTurnsAttempted;
    bool usedAbilityThisTurn;
    bool shieldActive;
    int discountPercent;
    int discountDuration;
    int consecutiveDoubleCount;

public:
    Player(Account* account, int money) : account(account), money(money), position(0), status(PlayerStatus::ACTIVE),
        ownedProperties({}), handCards({}), jailTurnsAttempted(0), usedAbilityThisTurn(false),
        shieldActive(false), discountPercent(0), discountDuration(0), consecutiveDoubleCount(0) {}
    Player(Account* account, int money, int position, PlayerStatus status, std::vector<PropertyTile*> ownedProperties,
        std::vector<std::unique_ptr<AbilityCard>> handCards, int jailTurnsAttempted, bool usedAbilityThisTurn, bool shieldActive,
        int discountPercent, int discountDuration, int consecutiveDoubleCount) : account(account), money(money),
        position(position), status(status), ownedProperties(ownedProperties), handCards(handCards),
        jailTurnsAttempted(jailTurnsAttempted), usedAbilityThisTurn(usedAbilityThisTurn), shieldActive(shieldActive),
        discountPercent(discountPercent), discountDuration(discountDuration), consecutiveDoubleCount(consecutiveDoubleCount) {}
    void moveTo(int position);
    void pay(int amount);
    void receive(int amount);
    bool canAfford(int amount) const;
    void addProperty(PropertyTile* prop);
    void removeProperty(PropertyTile* prop);
    void addCard(std::unique_ptr<AbilityCard> card);
    std::unique_ptr<AbilityCard> dropCard(int index);
    void useAbilityCard(int index, Game* game);
    void enterJail();
    void releaseFromJail();
    void incrementJailAttempt();
    void resetTurnFlags();
    int getTotalWealth() const;
    bool isBankrupt() const;
    std::string toString() const;
    bool operator<(const Player& other) const;
    bool operator>(const Player& other) const;
    bool operator==(const Player& other) const;
};
