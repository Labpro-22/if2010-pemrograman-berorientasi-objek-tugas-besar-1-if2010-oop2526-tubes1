#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H
#include "../models/GameBoard.hpp"
#include <vector>
using namespace std;

class GameBoard;
class TurnManager;
class MovementHandler;
class Player;
class SkillCardManager;
class Dice;

class GameContext {
public:
    GameBoard* board;
    MovementHandler* movementHandler;
    TurnManager* turnManager;
    SkillCardManager* skillCardManager;
    Dice* lastDice;
    std::vector<Player*> allPlayers;

    // Helper supaya kartu tidak perlu akses board langsung
    int getBoardSize() const {
        if (board != nullptr) return board->getTiles().size();
        return 40;
    }
};

#endif
