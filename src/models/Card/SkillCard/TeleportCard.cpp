#include "models/Card/SkillCard/TeleportCard.hpp"
#include "core/GameException.hpp"

TeleportCard::TeleportCard(int targetIndex) : targetIndex(targetIndex){}

TeleportCard::~TeleportCard() {}

void TeleportCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    int boardSize = ctx.getBoard().getSize();

    try {
        currPlayer.moveTo(targetIndex, boardSize);
        currPlayer.setUsedSkillThisTurn(true);
    } catch (const GameException& e) {
        std::cerr << e.what() << '\n';
    }
}

const string TeleportCard::getName() const{
    return "TeleportCard";
}

const string TeleportCard::getDescription() const {
    return "Memberikan pemain kebebasan untuk berpindah ke petak manapun di atas papan permainan.";
}
