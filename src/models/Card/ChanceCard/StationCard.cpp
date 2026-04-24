#include "models/Card/ChanceCard/StationCard.hpp"
#include "core/GameException.hpp"

const string StationCard::getName() const{
    return "StationCard";
}

const string StationCard::getDescription() const{
    return "Pergi ke stasiun terdekat";
}

void StationCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    Board& board = ctx.getBoard();
    int boardSize = board.getSize();
    int playerPosition = currPlayer.getPosition();

    try {
        // TODO: Index masih sama seperti spesifikasi, sesuaikan sama implementasi
        if (playerPosition >= board.findPlotIndex("TUG")) {
            currPlayer.moveTo(board.findPlotIndex("GBR"), boardSize);
        } else if (playerPosition >= board.findPlotIndex("STB")) {
            currPlayer.moveTo(board.findPlotIndex("TUG"), boardSize);
        } else if (playerPosition >= board.findPlotIndex("GBR")) {
            currPlayer.moveTo(board.findPlotIndex("STB"), boardSize);
        }
    } catch (const GameException& e) {
        std::cerr << e.what() << '\n';
    }
}



