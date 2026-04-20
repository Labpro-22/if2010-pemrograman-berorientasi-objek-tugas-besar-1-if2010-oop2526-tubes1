#pragma once
#include <random>
using namespace std;
class Dice {
private:
    int dice1;
    int dice2;
    bool isManual;

public:
    Dice();
    ~Dice();

    int roll();
    void setDiceManual(int d1, int d2);
    void resetManual();
    bool isDouble() const;
    int getDice1() const;
    int getDice2() const;
    int getTotal() const;
    bool getIsManual() const;
};