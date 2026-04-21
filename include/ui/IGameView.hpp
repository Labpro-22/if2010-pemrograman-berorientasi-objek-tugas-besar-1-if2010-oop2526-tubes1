#pragma once

#include <string>
#include <vector>

#include "ui/UiDtos.hpp"

class GameStateView;

class IGameView {
public:
    virtual ~IGameView() = default;

    virtual void showBoard(const GameStateView& boardState) = 0;
    virtual void showDiceResult(int d1, int d2, const string& playerName) = 0;
    virtual void showPlayerLanding(const string& playerName, const string& tileName) = 0;

    virtual void showPropertyCard(const PropertyInfo& propertyInfo) = 0;
    virtual void showPlayerProperties(const vector<PropertyInfo>& list) = 0;

    virtual void showBuyPrompt(const PropertyInfo& propertyInfo, Money playerMoney) = 0;
    virtual void showRentPayment(const RentInfo& rentInfo) = 0;
    virtual void showTaxPrompt(const TaxInfo& taxInfo) = 0;

    virtual void showAuctionState(const AuctionState& auctionState) = 0;
    virtual void showFestivalPrompt(const vector<PropertyInfo>& ownedProperties) = 0;

    virtual void showBankruptcy(const BankruptcyInfo& bankruptcyInfo) = 0;
    virtual void showLiquidationPanel(const LiquidationState& liquidationState) = 0;

    virtual void showCardDrawn(const CardInfo& cardInfo) = 0;
    virtual void showSkillCardHand(const vector<CardInfo>& cards) = 0;

    virtual void showTransactionLog(const vector<LogEntry>& entries) = 0;
    virtual void showWinner(const WinnerInfo& winInfo) = 0;

    virtual void showJailStatus(const JailInfo& jailInfo) = 0;

    virtual void showMessage(const string& message) = 0;

    virtual void showBuildMenu(const BuildMenuState& buildMenuState) = 0;
    virtual void showMortgageMenu(const MortgageMenuState& mortgageMenuState) = 0;
    virtual void showRedeemMenu(const RedeemMenuState& redeemMenuState) = 0;

    virtual void showDropCardPrompt(const vector<CardInfo>& cards) = 0;
    virtual void showSaveLoadStatus(const string& message) = 0;

    virtual void showTurnInfo(const string& playerName, int turnNum, int maxTurn) = 0;
};
