#include "models/Card/CommunityChestCard/DoctorCard.hpp"
 
string DoctorCard::getName() {
    return "DoctorCard";
}
 
string DoctorCard::getDescription() {
    return "Biaya dokter. Bayar M700.";
}
 
void DoctorCard::activate(SkillContext& ctx) {
    Player& currPlayer = ctx.getCurrentPlayer();
    currPlayer.pay(700);
}
 