#ifndef BOARD_VIEW_HPP
#define BOARD_VIEW_HPP

#include <vector>
#include <string>

class GameBoard;
class Player;
class Tile;
class Property;

class BoardView {
public:
    void showBoard(GameBoard& board, const std::vector<Player>& players);

private:
    std::string getPlayersOnTile(int pos, const std::vector<Player>& players);
    std::string getColorCode(const std::string& color);
    std::string renderTile(Tile* tile, const std::vector<Player>& players);
};

#endif