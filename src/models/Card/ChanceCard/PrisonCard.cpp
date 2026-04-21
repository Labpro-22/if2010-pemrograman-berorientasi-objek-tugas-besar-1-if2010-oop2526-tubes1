#include "models/Card/ChanceCard/PrisonCard.hpp"

string PrisonCard::getName() {
    return "PrisonCard";
}

string PrisonCard::getDescription() {
    return "Masuk Penjara";
}

void PrisonCard::activate(GameEngine& ge) {
     Player& currPlayer = ge.getState().getCurrentPlayer();
     int boardSize = ge.getState().getBoard().getSize();

     int index = ge.getState().getBoard().findPlotIndex("PEN");
     currPlayer.moveTo(index, boardSize);
     currPlayer.setJailTurns(3);
}



