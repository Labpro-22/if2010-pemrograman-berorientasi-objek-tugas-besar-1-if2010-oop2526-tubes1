#pragma once

#include <string>
#include <vector>

#include "models/Money.hpp"
#include "utils/Enums.hpp"


class Bank;
class FestivalManager;
class TransactionLogger;
class GameConfig;
class Dice;
class Player;
class Board;
class PropertyTile;

struct GameContext {
    Bank& bank;
    FestivalManager& festivalManager;
    TransactionLogger& logger;
    GameConfig& config;
    Dice& dice;
    vector<Player*>& players;
    Board& board;
    int currentTurn;
};

struct LandingResult {
    LandingAction action;
    int amount;
    Player* creditor;
    PropertyTile* property;
};

struct LogEntry {
    int turn;
    string username;
    string actionType;
    string detail;
};

struct AuctionResult {
    Player* winner;
    int finalPrice;
    PropertyTile* property;
};

struct FestivalEffect {
    Player* owner;
    int multiplier;
    int turnsRemaining;
    int timesApplied;
};

struct PlayerView {
    string username;
    Money money;
    int position;
    PlayerStatus status;
    int propertyCount;
    int skillCardCount;
};

struct PropertyView {
    string code;
    string name;
    string ownerName;
    PropertyStatus status;
    int buildingLevel;
};
