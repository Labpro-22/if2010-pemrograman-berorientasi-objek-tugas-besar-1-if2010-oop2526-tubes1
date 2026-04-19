#pragma once

#include "core/GameState.hpp"

class TurnManager {
public:
    void startTurn(Player& player, GameState& state);
    void endTurn(Player& player, GameState& state);
    bool handleExtraTurn(Player& player, bool rolledDouble, GameState& state);
    void advanceTurn(GameState& state);
};
