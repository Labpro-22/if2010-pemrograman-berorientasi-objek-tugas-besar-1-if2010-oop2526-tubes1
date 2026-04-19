#pragma once

#include <random>
#include <utility>

class Dice {
private:
    int die1;
    int die2;
    bool manualSet;
    std::mt19937 rng;

public:
    Dice();

    std::pair<int, int> roll();
    void setManual(int d1, int d2);

    int getDie1() const;
    int getDie2() const;
    int getTotal() const;
    bool isDouble() const;
};
