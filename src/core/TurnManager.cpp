#include "core/TurnManager.hpp"

TurnManager::TurnManager(int maxTurn)
    : currentIndex(0),
      currentTurn(1),
      maxTurn(maxTurn) {}
