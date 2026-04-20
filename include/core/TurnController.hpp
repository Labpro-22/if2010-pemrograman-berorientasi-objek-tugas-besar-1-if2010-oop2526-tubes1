#pragma once
#include <list>
#include "Player.hpp"

using namespace std;

class TurnController
{
private:
    list<Player> players;
    int currentPlayerIndex;
    int currentTurn;
    int maxTurns;
    int startingMoney;
public:
    void nextTurn();
    
    void setMaxTurns(int maxTurns);
    void setStartingMoney(int startingMoney);
    
    int getCurrentTurn();
    bool checkWinCondition();
    void exportGameState(); 
    void executeAction();
};

