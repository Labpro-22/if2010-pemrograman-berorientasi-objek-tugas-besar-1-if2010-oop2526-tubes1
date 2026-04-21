#include "models/Card/ChanceCard/BackwardMoveCard.hpp"

string BackwardMoveCard::getName() {
    return "BackwardMoveCard";
}

string BackwardMoveCard::getDescription() {
    return "Mundur 3 petak";
}

void BackwardMoveCard::activate(GameEngine& ge) {
    Player currPlayer = ge.getState().getCurrentPlayer();
    int boardSize = ge.getState().getBoard().getSize();
    currPlayer.move(-3, boardSize);
}
