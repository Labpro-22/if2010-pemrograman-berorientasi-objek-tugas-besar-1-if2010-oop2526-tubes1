#include "ui/GUIInput.hpp"

GUIInput::GUIInput(sf::RenderWindow& window) : window(&window) {}

int GUIInput::getMenuChoice(const vector<string>& options) {
    (void)options;
    return 0;
}

string GUIInput::getCommand() { return string{}; }

bool GUIInput::getYesNo(const string& prompt) {
    (void)prompt;
    return false;
}

int GUIInput::getNumberInRange(const string& prompt, int min, int max) {
    (void)prompt;
    (void)min;
    (void)max;
    return min;
}

string GUIInput::getString(const string& prompt) {
    (void)prompt;
    return string{};
}

pair<int, int> GUIInput::getManualDice() { return {1, 1}; }

AuctionAction GUIInput::getAuctionAction(
    const string& playerName, int currentBid, int playerMoney) {
    (void)playerName;
    (void)currentBid;
    (void)playerMoney;
    return AuctionAction::PASS;
}

TaxChoice GUIInput::getTaxChoice() { return TaxChoice::FLAT; }

int GUIInput::getLiquidationChoice(int numOptions) {
    (void)numOptions;
    return 0;
}

int GUIInput::getSkillCardChoice(int numCards) {
    (void)numCards;
    return 0;
}

string GUIInput::getPropertyCodeInput(const string& prompt) {
    (void)prompt;
    return string{};
}
