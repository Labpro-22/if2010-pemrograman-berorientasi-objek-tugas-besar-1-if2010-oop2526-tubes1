#include "models/Card/CommunityChestCard/DoctorCard.hpp"
#include "core/GameEngine.hpp"
 
string DoctorCard::getName() {
    return "DoctorCard";
}
 
string DoctorCard::getDescription() {
    return "Biaya dokter. Bayar M700.";
}
 
void DoctorCard::activate(GameEngine& ge) {
    Player& currPlayer = ge.getState().getCurrentPlayer();
    currPlayer.pay(700);
}
 