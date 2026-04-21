#include "models/Card/ChanceCard/StationCard.hpp"

StationCard::StationCard(){

}

StationCard::~StationCard() {

}

string StationCard::getName() {
    return "StationCard";
}

string StationCard::getDescription() {
    return "Pergi ke stasiun terdekat";
}

void StationCard::activate(GameEngine& ge) {
    int playerPosition = ge.getState().getCurrentPlayerIdx();
    GameState& state = ge.getState();
    // TODO: Index masih sama seperti spesifikasi, sesuaikan sama implementasi
    if (playerPosition >= 26) {
        state.setCurrentPlayerIdx(6);
    } else if (playerPosition >= 16) {
        state.setCurrentPlayerIdx(26);
    } else if (playerPosition >= 6) {
        state.setCurrentPlayerIdx(16);
    }
}



