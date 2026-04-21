#include "models/Card/SkillCard/TeleportCard.hpp"

TeleportCard::TeleportCard() {

}

TeleportCard::~TeleportCard() {

}

void TeleportCard::activate(GameEngine& ge) {
    // TODO: Sesuaikan dengan implementasi 
    int indexTarget;
    cout << "Index tujuan: ";
    cin >> indexTarget;
    cout << endl;
    ge.getState().setCurrentPlayerIdx(indexTarget);
}

string TeleportCard::getName() {
    return "TeleportCard";
}

string TeleportCard::getDescription() {
    return "Memberikan pemain kebebasan untuk berpindah ke petak manapun di atas papan permainan.";
}