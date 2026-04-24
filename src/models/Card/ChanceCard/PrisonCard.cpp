#include "models/Card/ChanceCard/PrisonCard.hpp"
#include "core/GameException.hpp"

const string PrisonCard::getName() const {
    return "PrisonCard";
}

const string PrisonCard::getDescription() const{
    return "Masuk Penjara";
}

void PrisonCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    int boardSize = ctx.getBoard().getSize();

    int index = ctx.getBoard().findPlotIndex("PEN");
    try {
        currPlayer.moveTo(index, boardSize);
        currPlayer.setJailTurns(3);
        currPlayer.setStatus(PlayerStatus::JAILED);
        currPlayer.resetConsecutiveDoubles();
        currPlayer.setHasRolled(true);
    } catch (const GameException& e) {
        std::cerr << e.what() << '\n';
    }
}



