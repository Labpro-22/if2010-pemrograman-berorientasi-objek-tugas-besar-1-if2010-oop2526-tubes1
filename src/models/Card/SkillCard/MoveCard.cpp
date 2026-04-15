#include "models/Card/SkillCard/MoveCard.hpp"

MoveCard::MoveCard() {

}

MoveCard::~MoveCard() {

}

void MoveCard::activate() {
     // TODO: 
}

string MoveCard::getName() {
    return "MoveCard";
}

string MoveCard::getDescription() {
    return "Memungkinkan pemain untuk bergerak maju sekian petak. Nominal langkah pergerakan ditentukan secara acak pada saat pemain pertama kali mendapatkan kartu ini.";
}