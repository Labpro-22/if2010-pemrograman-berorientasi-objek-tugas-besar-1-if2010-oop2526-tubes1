#include "controllers/HumanController.hpp"

HumanController::HumanController(IGameInput* input) : input(input) {}

string HumanController::chooseCommand(const GameStateView& state) {
    (void)state;
    return input->getCommand();
}

bool HumanController::decideBuyProperty(const PropertyInfo& info, Money money) {
    (void)info;
    (void)money;
    return input->getYesNo("Buy this property? (Y/N)");
}

AuctionAction HumanController::decideAuction(int currentBid, Money money) {
    return input->getAuctionAction("Player", currentBid, money.getAmount());
}

TaxChoice HumanController::decideTax(int flatAmount, int percentAmount) {
    (void)flatAmount;
    (void)percentAmount;
    return input->getTaxChoice();
}

int HumanController::decideSkillCard(const vector<CardInfo>& cards) {
    return input->getSkillCardChoice(static_cast<int>(cards.size()));
}

string HumanController::decideFestivalProperty(const vector<PropertyInfo>& props) {
    (void)props;
    return input->getPropertyCodeInput("Choose property code");
}

int HumanController::decideBuild(const BuildMenuState& state) {
    return input->getMenuChoice(state.options);
}

int HumanController::decideLiquidation(const LiquidationState& state) {
    return input->getLiquidationChoice(static_cast<int>(state.options.size()));
}

int HumanController::decideDropCard(const vector<CardInfo>& cards) {
    return input->getSkillCardChoice(static_cast<int>(cards.size()));
}

bool HumanController::decideJailPay() {
    return input->getYesNo("Pay jail fine? (Y/N)");
}

string HumanController::decideTeleportTarget() {
    return input->getPropertyCodeInput("Teleport to tile code");
}

string HumanController::decideLassoTarget(const vector<string>& players) {
    (void)players;
    return input->getString("Lasso target player name");
}

string HumanController::decideDemolitionTarget(const vector<PropertyInfo>& properties) {
    (void)properties;
    return input->getPropertyCodeInput("Demolition target property code");
}
