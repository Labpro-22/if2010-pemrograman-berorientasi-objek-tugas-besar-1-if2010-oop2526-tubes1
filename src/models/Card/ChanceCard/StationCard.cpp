#include "models/Card/ChanceCard/StationCard.hpp"

string StationCard::getName() {
    return "StationCard";
}

string StationCard::getDescription() {
    return "Pergi ke stasiun terdekat";
}

void StationCard::activate(GameEngine& ge) {
    int playerPosition = ge.getState().getCurrentPlayerIdx();
    Player& currPlayer = ge.getState().getCurrentPlayer();
    Board& board = ge.getState().getBoard();
    int boardSize = board.getSize();
    const vector<unique_ptr<Plot>>& plots = board.getPlots();
    
    // TODO: Index masih sama seperti spesifikasi, sesuaikan sama implementasi
    if (playerPosition >= board.findPlotIndex("TUG")) {
        currPlayer.moveTo(board.findPlotIndex("GBR"), boardSize);
    } else if (playerPosition >= board.findPlotIndex("STB")) {
        currPlayer.moveTo(board.findPlotIndex("TUG"), boardSize);
    } else if (playerPosition >= board.findPlotIndex("GBR")) {
        currPlayer.moveTo(board.findPlotIndex("STB"), boardSize);
    }
}



