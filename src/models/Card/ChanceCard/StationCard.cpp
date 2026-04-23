#include "models/Card/ChanceCard/StationCard.hpp"

const string StationCard::getName() const{
    return "StationCard";
}

const string StationCard::getDescription() const{
    return "Pergi ke stasiun terdekat";
}

void StationCard::activate(GameState& state) {
    int playerPosition = state.getCurrentPlayerIdx();
    Player& currPlayer = state.getCurrentPlayer();
    Board& board = state.getBoard();
    int boardSize = board.getSize();
    const vector<unique_ptr<Plot>>& plots = board.getPlots();

    try {
        // TODO: Index masih sama seperti spesifikasi, sesuaikan sama implementasi
        if (playerPosition >= board.findPlotIndex("TUG")) {
            currPlayer.moveTo(board.findPlotIndex("GBR"), boardSize);
        } else if (playerPosition >= board.findPlotIndex("STB")) {
            currPlayer.moveTo(board.findPlotIndex("TUG"), boardSize);
        } else if (playerPosition >= board.findPlotIndex("GBR")) {
            currPlayer.moveTo(board.findPlotIndex("STB"), boardSize);
        }
    } catch(const std::invalid_argument& e){
        std::cerr << e.what() << '\n';
    } catch(const std::out_of_range& e) {
        std::cerr << e.what() << '\n';
    }
}



