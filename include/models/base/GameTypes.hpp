#pragma once
#include <map>
#include <string>
#include <vector>

enum class PropertyType {
    STREET,
    RAILROAD,
    UTILITY
};

enum class PropertyStatus {
    BANK,
    OWNED,
    MORTGAGED
};

enum class PlayerStatus {
    ACTIVE,
    JAILED,
    BANKRUPT
};

enum class TurnPhase {
    START_TURN,
    PRE_ROLL,
    JAIL_CHOICE,
    POST_RESOLUTION,
    END_TURN
};

enum class ActionType {
    CHANCE,
    COMMUNITY_CHEST,
    FESTIVAL,
    TAX_PPH,
    TAX_PBM
};

enum class SpecialType {
    GO,
    JAIL,
    FREE_PARKING,
    GO_TO_JAIL
};

enum class CardType {
    CHANCE,
    COMMUNITY_CHEST,
    SKILL
};

enum class CardEffectType {
    RECEIVE_MONEY,
    PAY_MONEY,
    MOVE_RELATIVE,
    MOVE_TO_POSITION,
    MOVE_TO_NEAREST_RAILROAD,
    GO_TO_JAIL,
    GET_OUT_OF_JAIL,
    PAY_EACH_PLAYER,
    RECEIVE_FROM_EACH_PLAYER
};

class PropertyDefinition {
public:
    int id;
    std::string code;
    std::string name;
    PropertyType type;
    std::string colorGroup;
    int purchasePrice;
    int mortgageValue;
    int houseCost;
    int hotelCost;
    int colorGroupSize;
    std::vector<int> rentTable;

    PropertyDefinition();
};

class ActionTileDefinition {
public:
    int id;
    std::string code;
    std::string name;
    std::string colorGroup;
    bool isSpecial;
    ActionType actionType;
    SpecialType specialType;

    ActionTileDefinition();
};

class TaxConfig {
public:
    int pphFlat;
    int pphPercentage;
    int pbmFlat;

    TaxConfig();
};

class SpecialConfig {
public:
    int goSalary;
    int jailFine;

    SpecialConfig();
};

class MiscConfig {
public:
    int maxTurn;
    int startingBalance;

    MiscConfig();
};

class GameConfig {
public:
    std::vector<PropertyDefinition> properties;
    std::vector<ActionTileDefinition> actionTiles;
    std::map<int, int> railroadRentByCount;
    std::map<int, int> utilityMultiplierByCount;
    TaxConfig tax;
    SpecialConfig special;
    MiscConfig misc;

    GameConfig();
};

class SavedSkillCardState {
public:
    std::string cardName;
    int value;
    int remainingDuration;

    SavedSkillCardState();
};

class SavedPropertyState {
public:
    std::string code;
    std::string ownerName;
    PropertyType propertyType;
    PropertyStatus status;
    int buildingLevel;
    int festivalMultiplier;
    int festivalTurnsRemaining;

    SavedPropertyState();
};

class SavedPlayerState {
public:
    std::string name;
    int money;
    int position;
    std::string tileCode;
    PlayerStatus status;
    int jailTurnsSpent;
    std::vector<SavedSkillCardState> skillCards;

    SavedPlayerState();
};

class SavedDeckState {
public:
    std::vector<std::string> skillDeckCardNames;

    SavedDeckState();
};

class LogEntry {
public:
    int turnNumber;
    std::string username;
    std::string actionName;
    std::string detail;

    LogEntry();
    LogEntry(int turnNumber,
             const std::string& username,
             const std::string& actionName,
             const std::string& detail);
};

class GameSaveData {
public:
    int currentTurn;
    int maxTurn;
    std::vector<SavedPlayerState> players;
    std::vector<std::string> turnOrderNames;
    int currentPlayerIndex;
    std::vector<SavedPropertyState> properties;
    SavedDeckState skillDeckState;
    std::vector<LogEntry> logEntries;

    GameSaveData();
};
