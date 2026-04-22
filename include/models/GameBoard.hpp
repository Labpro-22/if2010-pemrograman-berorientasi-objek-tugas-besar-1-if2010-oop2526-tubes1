#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Tile.hpp"
#include "Player.hpp"

class Tile;
class Player;


class GameBoard {
private:
    // std::vector<Tile*> tiles;
    // std::vector<Player*> players;
    std::vector<std::unique_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<Player>> players;
    int currentPlayerIndex;

public:
    GameBoard();

    
    void addTile(std::unique_ptr<Tile> tile);
    Tile* getTileAt(int position) const;

    void addPlayer(std::shared_ptr<Player> player);
    std::shared_ptr<Player> getPlayerByUsername(const std::string& username) const;
    std::shared_ptr<Player> getCurrentPlayer() const;

    void nextTurn();

    const std::vector<std::shared_ptr<Player>>& getPlayers() const;
    const std::vector<std::unique_ptr<Tile>>& getTiles() const;

};
