#pragma once

#include <string>
#include <vector>

#include "models/Money.hpp"
#include "utils/Enums.hpp"
#include "utils/Structs.hpp"

// Lightweight DTOs for UI layer.
// Keep these free from heavy model includes to avoid circular dependencies.

struct PropertyInfo {
    string code;
    string name;
    string ownerName;  // empty if BANK
    PropertyStatus status{PropertyStatus::BANK};
    int buildingLevel{0};
    Money purchasePrice{0};
};

struct RentInfo {
    string payerName;
    string ownerName;
    Money amount{0};
    PropertyInfo property;
};

struct TaxInfo {
    int flatAmount{0};
    int percentageAmount{0};
};

struct AuctionState {
    int currentBid{0};
    string highestBidderName;
    vector<string> participantNames;
    PropertyInfo property;
};

struct BankruptcyInfo {
    string playerName;
    Money obligation{0};
    string creditorName;  // empty if BANK
};

struct LiquidationState {
    Money obligation{0};
    Money maxLiquidation{0};
    vector<string> options;
};

struct CardInfo {
    string name;
    string description;
    string type;
};

struct WinnerInfo {
    vector<string> winners;
};

struct JailInfo {
    string playerName;
    int turnsRemaining{0};
    int fine{0};
};

struct BuildMenuState {
    vector<string> options;
};

struct MortgageMenuState {
    vector<string> options;
};

struct RedeemMenuState {
    vector<string> options;
};
