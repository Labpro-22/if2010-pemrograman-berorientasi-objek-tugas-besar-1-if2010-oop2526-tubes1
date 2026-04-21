#include "models/Card/SkillCard/TeleportCard.hpp"

TeleportCard::TeleportCard(int targetIndex) : targetIndex(targetIndex){}

TeleportCard::~TeleportCard() {}

void TeleportCard::activate(GameEngine& ge) {
    Player& currPlayer = ge.getState().getCurrentPlayer();
    int boardSize = ge.getState().getBoard().getSize();
    
    currPlayer.moveTo(targetIndex, boardSize);
    currPlayer.setUsedSkillThisTurn(true);
}

string TeleportCard::getName() {
    return "TeleportCard";
}

string TeleportCard::getDescription() {
    return "Memberikan pemain kebebasan untuk berpindah ke petak manapun di atas papan permainan.";
}