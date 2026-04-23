#pragma once
#include <functional>
#include <memory>
#include <cstddef>
#include <string>
#include <vector>
#include "models/Player/PlayerStatus.hpp"

class Plot;
class SkillCard;
struct SkillContext;

class Player {
private:
    std::string username;
    int cash;
    int position;
    PlayerStatus status;
    int jailTurns;
    std::vector<std::reference_wrapper<Plot>> ownedProperties;
    std::vector<std::shared_ptr<SkillCard>> ownedCards;
    bool hasRolled;
    bool shieldActive;
    bool usedSkillThisTurn;
    int shieldTurnLeft;
    int discountTurnLeft;
    int discountValue;

public:
    Player();
    Player(std::string username, int startingCash, int startPosition = 0);

    const std::string& getUsername() const;
    int getCash() const;
    int getPosition() const;
    PlayerStatus getStatus() const;
    int getJailTurns() const;
    const std::vector<std::reference_wrapper<Plot>>& getOwnedProperties() const;
    const std::vector<std::shared_ptr<SkillCard>>& getOwnedCards() const;
    bool getHasRolled() const;
    bool isShieldActive() const;
    bool hasUsedSkillThisTurn() const;
    int getShieldTurnLeft() const;
    int getDiscountTurnLeft() const;
    int getDiscountValue() const;
    int getTotalWealth() const;

    void move();
    void move(int steps, int boardSize = 40);
    void moveTo(int index, int boardSize = 40);
    void pay(int amount);
    void payTaxes();
    bool buyProperty(Plot& property);
    bool useCards(std::size_t cardIndex, SkillContext& ctx);
    bool dropCard();
    bool dropCard(std::size_t cardIndex);
    void receive(int amount);

    Player& operator+=(int amount);
    Player& operator-=(int amount);
    bool operator<(Player& other);
    bool operator>(Player& other);

    void setStatus(PlayerStatus newStatus);
    void setJailTurns(int turns);
    void setHasRolled(bool value);
    void setShieldActive(bool value);
    void setShieldTurnLeft(int turns);
    void setDiscountTurnLeft(int turns);
    void setDiscountValue(int value);
    void decrementJailTurns();
    void decrementShieldTurn();
    void decrementDiscountTurn();
    void addOwnedCard(const std::shared_ptr<SkillCard>& card);
    void setUsedSkillThisTurn(bool used);
    void resetTurnFlags();

    bool isBankrupt() const;
};
