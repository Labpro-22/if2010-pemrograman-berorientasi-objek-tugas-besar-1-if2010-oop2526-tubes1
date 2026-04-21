#pragma once

#include "ui/IGameInput.hpp"

namespace sf {
class RenderWindow;
}

class GUIInput final : public IGameInput {
public:
    explicit GUIInput(sf::RenderWindow& window);

    int getMenuChoice(const vector<string>& options) override;
    string getCommand() override;

    bool getYesNo(const string& prompt) override;
    int getNumberInRange(const string& prompt, int min, int max) override;
    string getString(const string& prompt) override;

    pair<int, int> getManualDice() override;

    AuctionAction getAuctionAction(
        const string& playerName, int currentBid, int playerMoney) override;

    TaxChoice getTaxChoice() override;
    int getLiquidationChoice(int numOptions) override;
    int getSkillCardChoice(int numCards) override;

    string getPropertyCodeInput(const string& prompt) override;

private:
    [[maybe_unused]] sf::RenderWindow* window;
};
