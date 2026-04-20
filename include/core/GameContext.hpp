#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include <vector>
using namespace std;

class GameBoard;
class TurnManager;
class MovementHandler;
class Player;
class SkillCardManager;

class GameContext {
public:
    GameBoard* board;
    TurnManager* turnManager;
    MovementHandler* movementHandler;
    SkillCardManager* skillCardManager;

    vector<Player*> allPlayers;
};

#endif
