#include "models/Board/Dice.hpp"

#include <stdexcept>

Dice::Dice() : die1(1), die2(1), manualSet(false), rng(std::random_device{}()) {}

std::pair<int, int> Dice::roll() {
    if (!manualSet) {
        std::uniform_int_distribution<int> dist(1, 6);
        die1 = dist(rng);
        die2 = dist(rng);
    } else {
        manualSet = false;
    }

    return {die1, die2};
}

void Dice::setManual(int d1, int d2) {
    if (d1 < 1 || d1 > 6 || d2 < 1 || d2 > 6) {
        throw std::invalid_argument("Nilai dadu harus di antara 1 sampai 6.");
    }

    die1 = d1;
    die2 = d2;
    manualSet = true;
}

int Dice::getDie1() const {
    return die1;
}

int Dice::getDie2() const {
    return die2;
}

int Dice::getTotal() const {
    return die1 + die2;
}

bool Dice::isDouble() const {
    return die1 == die2;
}
