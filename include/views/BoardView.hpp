#ifndef BOARDVIEW_HPP
#define BOARDVIEW_HPP

#include <vector>
#include <memory>

class Player;
class Petak;

class BoardView {
public:
    static void printBoard(const std::vector<std::unique_ptr<Petak>>& board, const std::vector<Player>& players, int turnCount);
};

#endif
