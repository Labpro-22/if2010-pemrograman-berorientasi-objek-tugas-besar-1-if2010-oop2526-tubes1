#include "models/Card/CommunityChestCard/BirthdayCard.hpp"
#include "core/GameEngine.hpp"

string BirthdayCard::getName() {
    return "BirthdayCard";
}
 
string BirthdayCard::getDescription() {
    return "Ini adalah hari ulang tahun Anda. Dapatkan M100 dari setiap pemain.";
}
 
void BirthdayCard::activate(GameEngine& ge) {
    Player& currPlayer = ge.getState().getCurrentPlayer();
 
    for (Player& other : ge.getState().getPlayers()) {
        if (other.getUsername() == currPlayer.getUsername()) continue;
        if (other.isBankrupt()) continue;
 
        other.pay(100);
        currPlayer.receive(100);
    }
}
 