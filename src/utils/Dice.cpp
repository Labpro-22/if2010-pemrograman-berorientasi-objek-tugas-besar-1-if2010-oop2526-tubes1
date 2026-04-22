#include <cstdlib>
#include "../../include/utils/Dice.hpp"

Dice::Dice() {
    d1 = d2 = 1;
    isManualMode = false;
}

void Dice::rollRandom() {
    d1 = rand() % 6 + 1;
    d2 = rand() % 6 + 1;
    isManualMode = false;
}

void Dice::setManual(int a, int b) {
    d1 = a;
    d2 = b;
    isManualMode = true;
}

int Dice::getTotal() { return d1 + d2; }
bool Dice::isDouble() { return d1 == d2; }

int Dice::getD1() { return d1; }
int Dice::getD2() { return d2; }
int Dice::getDie1() { return d1; }
int Dice::getDie2() { return d2; }
bool Dice::getManualMode() { return isManualMode; }
