#include "models/Card/SkillCard/ShieldCard.hpp"

void ShieldCard::activate(SkillContext& ctx) {
    ctx.getCurrentPlayer().setShieldActive(true);
    ctx.getCurrentPlayer().setUsedSkillThisTurn(true);
    ctx.getCurrentPlayer().setShieldTurnLeft(1);
}

const string ShieldCard::getName() const {
    return "ShieldCard";
}

const string ShieldCard::getDescription() const {
    return "Melindungi pemain dari tagihan sewa maupun sanksi apapun yang merugikan selama 1 giliran.";
}