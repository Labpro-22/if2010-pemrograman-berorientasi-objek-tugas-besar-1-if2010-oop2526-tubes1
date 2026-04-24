#include "models/Card/CommunityChestCard/BirthdayCard.hpp"

string BirthdayCard::getName() {
    return "BirthdayCard";
}
 
string BirthdayCard::getDescription() {
    return "Ini adalah hari ulang tahun Anda. Dapatkan M100 dari setiap pemain.";
}
 
void BirthdayCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
 
    for (Player& other : ctx.getPlayers()) {
        if (other.getUsername() == currPlayer.getUsername()) continue;
        if (other.isBankrupt()) continue;
 
        other.pay(100);
        currPlayer.receive(100);
    }
}
 