#include "models/Card/ChanceCard/PrisonCard.hpp"

PrisonCard::PrisonCard(){

}

PrisonCard::~PrisonCard() {

}

string PrisonCard::getName() {
    return "PrisonCard";
}

string PrisonCard::getDescription() {
    return "Masuk Penjara";
}

void PrisonCard::activate(GameEngine& ge) {
     // TODO: Sesuaikan dengan implementasi (Di spesifikasi Penjara index 11)
     ge.getState().setCurrentPlayerIdx(11);
}



