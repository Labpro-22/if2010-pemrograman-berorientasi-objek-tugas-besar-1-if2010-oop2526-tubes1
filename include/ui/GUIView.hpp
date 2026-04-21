#pragma once

#include "ui/IGameView.hpp"

namespace sf {
class RenderWindow;
}

class GUIView final : public IGameView {
public:
    explicit GUIView(sf::RenderWindow& window);

    void showBoard(const GameStateView& boardState) override;
    void showDiceResult(int d1, int d2, const string& playerName) override;
    void showPlayerLanding(const string& playerName, const string& tileName) override;

    void showPropertyCard(const PropertyInfo& propertyInfo) override;
    void showPlayerProperties(const vector<PropertyInfo>& list) override;

    void showBuyPrompt(const PropertyInfo& propertyInfo, Money playerMoney) override;
    void showRentPayment(const RentInfo& rentInfo) override;
    void showTaxPrompt(const TaxInfo& taxInfo) override;

    void showAuctionState(const AuctionState& auctionState) override;
    void showFestivalPrompt(const vector<PropertyInfo>& ownedProperties) override;

    void showBankruptcy(const BankruptcyInfo& bankruptcyInfo) override;
    void showLiquidationPanel(const LiquidationState& liquidationState) override;

    void showCardDrawn(const CardInfo& cardInfo) override;
    void showSkillCardHand(const vector<CardInfo>& cards) override;

    void showTransactionLog(const vector<LogEntry>& entries) override;
    void showWinner(const WinnerInfo& winInfo) override;

    void showJailStatus(const JailInfo& jailInfo) override;

    void showMessage(const string& message) override;

    void showBuildMenu(const BuildMenuState& buildMenuState) override;
    void showMortgageMenu(const MortgageMenuState& mortgageMenuState) override;
    void showRedeemMenu(const RedeemMenuState& redeemMenuState) override;

    void showDropCardPrompt(const vector<CardInfo>& cards) override;
    void showSaveLoadStatus(const string& message) override;

    void showTurnInfo(const string& playerName, int turnNum, int maxTurn) override;

private:
    [[maybe_unused]] sf::RenderWindow* window;
};
