#pragma once
#include <string>
#include <vector>
#include "models/Plot/Colors.hpp"


class Board;
class Player;

class Formatter {
private:
static const int CELL_WIDTH = 8;

    static std::string colorToAnsi(Color color);
    static std::string ansiReset();

    static std::string getPlayerLabels(int plotIndex, const Board& board, const std::vector<Player>& players);
    static std::string makeCellContent(int plotIndex, const Board& board, const std::vector<Player>& players);
    static std::string renderCell(int plotIndex, const Board& board, const std::vector<Player>& players);
    static std::string makeSeparator(int numCells);
    static void calculateSides(int totalPlots, int& topCount, int& rightCount, int& bottomCount, int& leftCount);
    static std::string formatLegend(const std::vector<Player>& players, int currentTurn, int maxTurn);

public:
    static std::string boardFormat(const Board& board, const std::vector<Player>& players, int currentTurn, int maxTurn);
};

