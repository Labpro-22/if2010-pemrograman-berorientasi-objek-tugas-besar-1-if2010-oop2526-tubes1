#ifndef MOVEMENTHANDLER_H
#define MOVEMENTHANDLER_H

class GameBoard;
class Player;
class Dice;

enum JailResult {
    ESCAPED_DOUBLE,
    STILL_JAILED,
    FORCED_OUT
};

class MovementHandler {
private:
    GameBoard* board; //placehold
    int boardSize;
    int goSalary;
    int jailFine;

public:
    // Placeholder fallback: boardSize dan goSalary akan dipasok dari config/CLI saat integrasi penuh.
    MovementHandler(GameBoard* b, int boardSize = 40, int goSalary = 200, int jailFine = 50);

    void movePlayer(Player* player, int steps);
    void teleportPlayer(Player* player, int target);
    void pullPlayer(Player* target, int toPosition);

    void handlePassGo(Player* player);
    void sendToJail(Player* player);

    JailResult handleJailTurn(Player* player, Dice& dice);
};

#endif
