#pragma once
#include <vector>
#include <string>

class Tile;
class Player;

class GameBoard {
private:
    std::vector<Tile*> tiles;
    std::vector<Player*> players;
    int currentPlayerIndex;

public:
    GameBoard();

    
    void addTile(Tile* tile);
    Tile* getTileAt(int position) const;

    void addPlayer(Player* player);
    Player* getPlayerByUsername(const std::string& username) const;
    Player* getCurrentPlayer() const;

    void nextTurn();

    const std::vector<Player*>& getPlayers() const;
    const std::vector<Tile*>& getTiles() const;

};
