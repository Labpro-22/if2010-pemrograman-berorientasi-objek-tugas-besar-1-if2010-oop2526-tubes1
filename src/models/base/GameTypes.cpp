#include "models/base/GameTypes.hpp"

PropertyDefinition::PropertyDefinition()
    : id(0), type(PropertyType::STREET), purchasePrice(0), mortgageValue(0),
      houseCost(0), hotelCost(0), colorGroupSize(0) {}

ActionTileDefinition::ActionTileDefinition()
    : id(0),
      colorGroup("DEFAULT"),
      isSpecial(false),
      actionType(ActionType::COMMUNITY_CHEST),
      specialType(SpecialType::FREE_PARKING) {}

TaxConfig::TaxConfig() : pphFlat(150), pphPercentage(10), pbmFlat(200) {}

SpecialConfig::SpecialConfig() : goSalary(200), jailFine(50) {}

MiscConfig::MiscConfig() : maxTurn(15), startingBalance(1000) {}

GameConfig::GameConfig() {}

SavedSkillCardState::SavedSkillCardState()
    : value(0), remainingDuration(0) {}

SavedPropertyState::SavedPropertyState()
    : propertyType(PropertyType::STREET), status(PropertyStatus::BANK),
      buildingLevel(0), festivalMultiplier(1), festivalTurnsRemaining(0) {}

SavedPlayerState::SavedPlayerState()
    : money(0), position(1), status(PlayerStatus::ACTIVE), jailTurnsSpent(0) {}

SavedDeckState::SavedDeckState() {}

LogEntry::LogEntry() : turnNumber(0) {}

LogEntry::LogEntry(int turnNumber,
                   const std::string& username,
                   const std::string& actionName,
                   const std::string& detail)
    : turnNumber(turnNumber), username(username), actionName(actionName), detail(detail) {}

GameSaveData::GameSaveData() : currentTurn(1), maxTurn(15), currentPlayerIndex(0) {}
