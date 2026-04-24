#include "core/BoardFormatter.hpp"
#include "models/Board/Board.hpp"
#include "models/Player/Player.hpp"
#include "models/Player/PlayerStatus.hpp"
#include "models/Plot/PropertyPlot/PropertyPlot.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include <sstream>
#include <algorithm>
using namespace std;

string BoardFormatter::colorToAnsi(Color color) {
    switch (color) {
        case Color::BROWN:     return "\033[33m";
        case Color::LIGHTBLUE: return "\033[96m";
        case Color::PINK:      return "\033[95m";
        case Color::GRAY:      return "\033[90m";
        case Color::ORANGE:    return "\033[91m";
        case Color::RED:       return "\033[31m";
        case Color::YELLOW:    return "\033[93m";
        case Color::GREEN:     return "\033[32m";
        case Color::DARKBLUE:  return "\033[34m";
        default:               return "";
    }
}

string BoardFormatter::ansiReset() {
    return "\033[0m";
}

string BoardFormatter::getPlayerLabels(int plotIndex, const Board& board, const vector<Player>& players) {
    string label = "";
    int prisonIndex = board.findPlotIndex("PEN");

    for (int i = 0; i < static_cast<int>(players.size()); ++i) {
        const Player& p = players[i];
        if (p.isBankrupt()) continue;
        if (p.getPosition() != plotIndex) continue;

        if (plotIndex == prisonIndex) {
            label += (p.getStatus() == PlayerStatus::JAILED) ? "IN" : "V";
        } else {
            label += "(" + to_string(i + 1) + ")";
        }
    }
    return label;
}

string BoardFormatter::makeCellContent(int plotIndex, const Board& board, const vector<Player>& players) {
    const auto& plots = board.getPlots();
    if (plotIndex < 0 || plotIndex >= static_cast<int>(plots.size())) {
        return string(CELL_WIDTH, ' ') + "\n" + string(CELL_WIDTH, ' ');
    }

    Plot* plot = plots[plotIndex].get();
    string code = plot->getCode();
    if (static_cast<int>(code.size()) > CELL_WIDTH - 2)
        code = code.substr(0, CELL_WIDTH - 2);

    string buildingLabel = "";
    LandPlot* land = dynamic_cast<LandPlot*>(plot);
    if (land) {
        int level = land->getLevel();
        if      (level == 1) buildingLabel = "^";
        else if (level == 2) buildingLabel = "^^";
        else if (level == 3) buildingLabel = "^^^";
        else if (level == 4) buildingLabel = "^^^^";
        else if (level == 5) buildingLabel = "*";
    }

    string ownerLabel    = "";
    string mortgageLabel = "";
    PropertyPlot* prop = dynamic_cast<PropertyPlot*>(plot);
    if (prop) {
        Player* owner = prop->getOwner();
        if (owner != nullptr) {
            for (int i = 0; i < static_cast<int>(players.size()); ++i) {
                if (players[i].getUsername() == owner->getUsername()) {
                    ownerLabel = "P" + to_string(i + 1);
                    break;
                }
            }
        }
        if (prop->isMortgaged()) mortgageLabel = "[M]";
    }

    string playerLabel = getPlayerLabels(plotIndex, board, players);

    string line2 = "";
    if (!ownerLabel.empty())    line2 += ownerLabel;
    if (!buildingLabel.empty()) { if (!line2.empty()) line2 += " "; line2 += buildingLabel; }
    if (!mortgageLabel.empty()) line2 += mortgageLabel;
    if (!playerLabel.empty())   { if (!line2.empty()) line2 += " "; line2 += playerLabel; }

    auto pad = [](string s, int w) {
        if (static_cast<int>(s.size()) >= w) return s.substr(0, w);
        return s + string(w - s.size(), ' ');
    };
    return pad(code, CELL_WIDTH) + "\n" + pad(line2, CELL_WIDTH);
}

string BoardFormatter::renderCell(int plotIndex, const Board& board, const vector<Player>& players) {
    const auto& plots = board.getPlots();
    if (plotIndex < 0 || plotIndex >= static_cast<int>(plots.size())) {
        return "│" + string(CELL_WIDTH, ' ') + "│\n│" + string(CELL_WIDTH, ' ') + "│";
    }

    Plot* plot = plots[plotIndex].get();
    string ansi  = colorToAnsi(plot->getColor());
    string reset = ansi.empty() ? "" : ansiReset();
    string content = makeCellContent(plotIndex, board, players);

    istringstream ss(content);
    string line1, line2;
    getline(ss, line1);
    getline(ss, line2);

    return "│" + ansi + line1 + reset + "│\n│" + ansi + line2 + reset + "│";
}

string BoardFormatter::makeSeparator(int numCells) {
    string sep = "+";
    for (int i = 0; i < numCells; ++i) sep += string(CELL_WIDTH, '-') + "+";
    return sep;
}

void BoardFormatter::calculateSides(int totalPlots, int& topCount, int& rightCount, int& bottomCount, int& leftCount) {
    int base = totalPlots / 4;
    int rem  = totalPlots % 4;
    topCount    = base + (rem > 0 ? 1 : 0);
    rightCount  = base + (rem > 1 ? 1 : 0);
    bottomCount = base + (rem > 2 ? 1 : 0);
    leftCount   = totalPlots - topCount - rightCount - bottomCount;
}

string BoardFormatter::formatLegend(const vector<Player>& players, int currentTurn, int maxTurn) {
    ostringstream oss;
    oss << string(38, '-') << "\n";
    oss << "LEGENDA KEPEMILIKAN & STATUS\n";
    for (int i = 0; i < static_cast<int>(players.size()); ++i) {
        if (!players[i].isBankrupt())
            oss << "P" << (i+1) << " : " << players[i].getUsername() << "\n";
    }
    oss << "^/^^/^^^/^^^^ : Rumah L1-L4\n";
    oss << "*             : Hotel\n";
    oss << "[M]           : Digadaikan\n";
    oss << "IN / V        : Tahanan / Mampir di penjara\n";
    oss << string(38, '-') << "\n";
    oss << "KODE WARNA:\n";
    oss << colorToAnsi(Color::BROWN)     << "[CK] Coklat"    << ansiReset() << "   ";
    oss << colorToAnsi(Color::RED)       << "[MR] Merah"     << ansiReset() << "\n";
    oss << colorToAnsi(Color::LIGHTBLUE) << "[BM] Biru Muda" << ansiReset() << " ";
    oss << colorToAnsi(Color::YELLOW)    << "[KN] Kuning"    << ansiReset() << "\n";
    oss << colorToAnsi(Color::PINK)      << "[PK] Pink"      << ansiReset() << "      ";
    oss << colorToAnsi(Color::GREEN)     << "[HJ] Hijau"     << ansiReset() << "\n";
    oss << colorToAnsi(Color::ORANGE)    << "[OR] Orange"    << ansiReset() << "    ";
    oss << colorToAnsi(Color::DARKBLUE)  << "[BT] Biru Tua"  << ansiReset() << "\n";
    oss << colorToAnsi(Color::GRAY)      << "[AB] Utilitas"  << ansiReset() << "  ";
    oss << "[DF] Aksi\n";
    oss << string(38, '-') << "\n";
    oss << "TURN " << currentTurn << " / " << maxTurn << "\n";
    oss << string(38, '-') << "\n";
    return oss.str();
}

string BoardFormatter::boardFormat(const Board& board, const vector<Player>& players, int currentTurn, int maxTurn) {
    const int totalPlots = board.getSize();
    if (totalPlots == 0) return "[Board] Papan belum diinisialisasi.\n";

    int topCount, rightCount, bottomCount, leftCount;
    calculateSides(totalPlots, topCount, rightCount, bottomCount, leftCount);

    ostringstream oss;

    oss << makeSeparator(topCount) << "\n";
    vector<vector<string>> topLines(topCount);
    for (int i = 0; i < topCount; ++i) {
        istringstream ss(renderCell(i, board, players));
        string l;
        while (getline(ss, l)) topLines[i].push_back(l);
    }
    int numRows = topLines.empty() ? 0 : static_cast<int>(topLines[0].size());
    for (int row = 0; row < numRows; ++row) {
        for (int i = 0; i < topCount; ++i)
            if (row < static_cast<int>(topLines[i].size()))
                oss << topLines[i][row];
        oss << "\n";
    }
    oss << makeSeparator(topCount) << "\n";

    int midRows     = max(rightCount, leftCount);
    string innerSpace = string((topCount - 2) * (CELL_WIDTH + 1) - 1, ' ');

    for (int row = 0; row < midRows; ++row) {
        int leftIdx = totalPlots - 1 - row;
        string leftCell = (row < leftCount)
            ? renderCell(leftIdx, board, players)
            : "│" + string(CELL_WIDTH, ' ') + "│\n│" + string(CELL_WIDTH, ' ') + "│";

        string leftLine1, leftLine2;
        istringstream lss(leftCell);
        getline(lss, leftLine1); getline(lss, leftLine2);

        int rightIdx = topCount + row;
        string rightCell = (row < rightCount)
            ? renderCell(rightIdx, board, players)
            : "│" + string(CELL_WIDTH, ' ') + "│\n│" + string(CELL_WIDTH, ' ') + "│";

        string rightLine1, rightLine2;
        istringstream rss(rightCell);
        getline(rss, rightLine1); getline(rss, rightLine2);

        oss << leftLine1 << " " << innerSpace << " " << rightLine1 << "\n";
        oss << leftLine2 << " " << innerSpace << " " << rightLine2 << "\n";
    }

    int bottomStart = topCount + rightCount;
    oss << makeSeparator(topCount) << "\n";

    vector<vector<string>> botLines(topCount);
    for (int i = 0; i < bottomCount; ++i) {
        int visPos = topCount - 1 - i;
        if (visPos >= 0 && visPos < topCount) {
            istringstream ss(renderCell(bottomStart + i, board, players));
            string l;
            while (getline(ss, l)) botLines[visPos].push_back(l);
        }
    }
    int botRows = botLines.empty() ? 0 : static_cast<int>(botLines[0].size());
    for (int row = 0; row < botRows; ++row) {
        for (int i = 0; i < topCount; ++i) {
            if (row < static_cast<int>(botLines[i].size()))
                oss << botLines[i][row];
            else
                oss << "│" << string(CELL_WIDTH, ' ') << "│";
        }
        oss << "\n";
    }
    oss << makeSeparator(topCount) << "\n";
    oss << formatLegend(players, currentTurn, maxTurn);

    return oss.str();
}
