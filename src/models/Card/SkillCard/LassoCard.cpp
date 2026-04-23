#include "models/Card/SkillCard/LassoCard.hpp"

void LassoCard::activate(GameState& state) {
    vector<Player>& players = state.getPlayers();
    int currPlayerPosition = state.getCurrentPlayer().getPosition();
    int boardSize = state.getBoard().getSize();

    Player* nearestPlayer = nullptr;
    int minDistance = boardSize + 1;
    for_each(players.begin(), players.end(), [&](Player& player){
        int distance = (player.getPosition() - currPlayerPosition + boardSize) % boardSize;
        
        if(distance > 0 && distance < minDistance) {
            minDistance = distance;
            nearestPlayer = &player;
        }
    });

    try {
        nearestPlayer->moveTo(currPlayerPosition, boardSize);
        state.getCurrentPlayer().setUsedSkillThisTurn(true);
    } catch(const std::invalid_argument& e){
        std::cerr << e.what() << '\n';
    } catch(const std::out_of_range& e) {
        std::cerr << e.what() << '\n';
    }
}

const string LassoCard::getName() const{
    return "LassoCard";
}

const string LassoCard::getDescription() const{
    return "Menarik satu pemain lawan yang berada di depan posisi pemain saat ini ke petak tempat pemain tersebut berada.";
}