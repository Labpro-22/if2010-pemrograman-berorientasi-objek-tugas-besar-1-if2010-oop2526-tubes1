#include "models/Card/ChanceCard/PrisonCard.hpp"

const string PrisonCard::getName() const {
    return "PrisonCard";
}

const string PrisonCard::getDescription() const{
    return "Masuk Penjara";
}

void PrisonCard::activate(GameState& state) {
    Player& currPlayer = state.getCurrentPlayer();
    int boardSize = state.getBoard().getSize();

    int index = state.getBoard().findPlotIndex("PEN");
    try {
        currPlayer.moveTo(index, boardSize);
        currPlayer.setJailTurns(3);
        currPlayer.setStatus(PlayerStatus::JAILED);
    } catch(const std::invalid_argument& e){
        std::cerr << e.what() << '\n';
    } catch(const std::out_of_range& e) {
        std::cerr << e.what() << '\n';
    }
}



