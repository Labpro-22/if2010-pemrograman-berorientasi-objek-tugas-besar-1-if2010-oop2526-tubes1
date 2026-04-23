#include "models/Card/ChanceCard/BackwardMoveCard.hpp"

const string BackwardMoveCard::getName() const {
    return "BackwardMoveCard";
}

const string BackwardMoveCard::getDescription() const {
    return "Mundur 3 petak";
}

void BackwardMoveCard::activate(GameState& state) {
    Player currPlayer = state.getCurrentPlayer();
    int boardSize = state.getBoard().getSize();
    currPlayer.move(-3, boardSize);
}
