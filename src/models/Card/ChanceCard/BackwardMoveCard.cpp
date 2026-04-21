#include "models/Card/ChanceCard/BackwardMoveCard.hpp"

BackwardMoveCard::BackwardMoveCard(){

}

BackwardMoveCard::~BackwardMoveCard() {

}

string BackwardMoveCard::getName() {
    return "BackwardMoveCard";
}

string BackwardMoveCard::getDescription() {
    return "Mundur 3 petak";
}

void BackwardMoveCard::activate(GameEngine& ge) {
    Player currPlayer = ge.getState().getCurrentPlayer();
    currPlayer.move(-3, 40);
}
