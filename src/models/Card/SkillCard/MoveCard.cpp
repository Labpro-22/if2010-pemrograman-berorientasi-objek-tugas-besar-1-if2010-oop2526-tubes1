#include "models/Card/SkillCard/MoveCard.hpp"

MoveCard::MoveCard() {

}

MoveCard::~MoveCard() {

}

void MoveCard::activate(GameEngine& ge) {
    random_device random;
    mt19937 g(random());
    uniform_int_distribution<> number(1,12);
    int moveNumber = number(g);

    Player currPlayer = ge.getState().getCurrentPlayer();
    currPlayer.move(moveNumber, 40);
}

string MoveCard::getName() {
    return "MoveCard";
}

string MoveCard::getDescription() {
    return "Memungkinkan pemain untuk bergerak maju sekian petak. Nominal langkah pergerakan ditentukan secara acak pada saat pemain pertama kali mendapatkan kartu ini.";
}