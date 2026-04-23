#include "models/Card/SkillCard/ShieldCard.hpp"

void ShieldCard::activate(GameState& state) {
    state.getCurrentPlayer().setShieldActive(true);
    state.getCurrentPlayer().setUsedSkillThisTurn(true);
    state.getCurrentPlayer().setShieldTurnLeft(1);
}

const string ShieldCard::getName() const {
    return "ShieldCard";
}

const string ShieldCard::getDescription() const {
    return "Melindungi pemain dari tagihan sewa maupun sanksi apapun yang merugikan selama 1 giliran.";
}