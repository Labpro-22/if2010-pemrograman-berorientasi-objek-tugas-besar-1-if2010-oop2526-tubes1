#include "Dice.hpp"
#include <cstdlib> // Untuk fungsi rand()

Dice::Dice() : daduVal1(0), daduVal2(0), playerConcecutiveDoubles(0) {}

void Dice::rollRandom() {
    // Menghasilkan angka acak antara 1 sampai 6
    daduVal1 = (rand() % 6) + 1;
    daduVal2 = (rand() % 6) + 1;
    
    // Cek apakah dadu double
    if (daduVal1 == daduVal2) {
        playerConcecutiveDoubles++;
    } else {
        playerConcecutiveDoubles = 0;
    }
}

void Dice::setManual(int v1, int v2) {
    // Validasi exception rentang 1-6 dilakukan di AturDaduCommand sebelum memanggil ini
    daduVal1 = v1;
    daduVal2 = v2;
    
    // Cek apakah dadu double
    if (daduVal1 == daduVal2) {
        playerConcecutiveDoubles++;
    } else {
        playerConcecutiveDoubles = 0;
    }
}

int Dice::getTotal() const {
    return daduVal1 + daduVal2;
}

int Dice::getConsecutiveDoubles() const {
    return playerConcecutiveDoubles;
}

void Dice::resetConsecutiveDouvles() {
    playerConcecutiveDoubles = 0;
}

int Dice::getDaduVal1() const {
    return daduVal1;
}

int Dice::getDaduVal2() const {
    return daduVal2;
}