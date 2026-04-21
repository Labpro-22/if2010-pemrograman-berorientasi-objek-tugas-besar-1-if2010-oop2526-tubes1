#include "models/Card/SkillCard/LassoCard.hpp"

LassoCard::LassoCard() {

}

LassoCard::~LassoCard() {

}

void LassoCard::activate(GameEngine& ge) {
    vector<Player>& players = ge.getState().getPlayers();
    int currPlayerPosition = ge.getState().getCurrentPlayerIdx();

    // Asumsi hanya bisa narik player 1 index setelah current player
    auto it = find_if(players.begin(), players.end(), [&](const Player& p) {
        return p.getPosition() - currPlayerPosition == 1 && p.getPosition() > currPlayerPosition;
    });

    if (it != players.end()) {
        it->moveTo(currPlayerPosition, 40);
    } else {
        // throw somethin
    }
}

string LassoCard::getName() {
    return "LassoCard";
}

string LassoCard::getDescription() {
    return "Menarik satu pemain lawan yang berada di depan posisi pemain saat ini ke petak tempat pemain tersebut berada.";
}