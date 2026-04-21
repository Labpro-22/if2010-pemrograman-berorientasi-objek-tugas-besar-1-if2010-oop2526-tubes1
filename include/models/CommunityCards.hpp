#pragma once

#include "ActionCard.hpp"

class BirthdayCard : public ActionCard {
private:
    int amountPerPlayer;

public:
    explicit BirthdayCard(int amountPerPlayer = 100);
    void apply(Player& player, GameEngine& game) override;
};

class DoctorFeeCard : public ActionCard {
private:
    int fee;

public:
    explicit DoctorFeeCard(int fee = 700);
    void apply(Player& player, GameEngine& game) override;
};

class ElectionCard : public ActionCard {
private:
    int amountPerPlayer;

public:
    explicit ElectionCard(int amountPerPlayer = 200);
    void apply(Player& player, GameEngine& game) override;
};
