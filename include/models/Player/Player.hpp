#pragma once
#include <string>
#include "models/Player/PlayerStatus.hpp"

class Player {
private:
    std::string username;
    int cash;
    int position;
    PlayerStatus status;
    int jailTurns;
    bool usedSkillThisTurn;

public:
    Player();
    Player(std::string username, int startingCash, int startPosition = 0);

    const std::string& getUsername() const;
    int getCash() const;
    int getPosition() const;
    PlayerStatus getStatus() const;
    int getJailTurns() const;
    bool hasUsedSkillThisTurn() const;

    void move(int steps, int boardSize = 40);
    void moveTo(int index, int boardSize = 40);
    void pay(int amount);
    void receive(int amount);

    void setStatus(PlayerStatus newStatus);
    void setJailTurns(int turns);
    void decrementJailTurns();
    void setUsedSkillThisTurn(bool used);
    void resetTurnFlags();

    bool isBankrupt() const;
};
