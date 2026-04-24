#pragma once

#include "core/GameState.hpp"

class TurnManager {
public:
    static constexpr int MAX_JAIL_TURNS = 3;
    static constexpr int MAX_CONSECUTIVE_DOUBLES = 3;
    static constexpr int JAIL_FINE = 50;

    void startTurn(Player& player, GameState& state);
    void endTurn(Player& player, GameState& state);
    bool handleExtraTurn(Player& player, bool rolledDouble, GameState& state);
    void advanceTurn(GameState& state);

    void sendToJail(Player& player, GameState& state);
    bool payJailFine(Player& player, GameState& state);
    bool handleJailedRoll(Player& player, bool rolledDouble, GameState& state);
};
