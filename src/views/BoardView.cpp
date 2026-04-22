#include "../../include/views/BoardView.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

string BoardView::formatTile(Tile* tile, const vector<shared_ptr<Player>>& players) {

    if (!tile) return "EMPTY";

    string code = tile->getCode();   // contoh: MR, DF, KN

    string name = tile->getName();
    if (name.length() > 3) name = name.substr(0,3);

    string color = getColorCode(tile->getColor());

    string playerStr = "";
    for (int i = 0; i < players.size(); i++) {
        if (players[i]->getPosition() == tile->getPosition()) {
            playerStr += "P" + to_string(i+1) + " ";
        }
    }

    string result = color + "[" + code + "] " + name;

    if (!playerStr.empty()) {
        result += " " + playerStr;
    }

    result += "\033[0m"; // reset warna

    return result;
}

string BoardView::getColorCode(const string& color) {
    if (color == "MERAH") return "\033[31m";
    if (color == "KUNING") return "\033[33m";
    if (color == "HIJAU") return "\033[32m";
    if (color == "BIRU_MUDA") return "\033[36m";
    if (color == "BIRU_TUA") return "\033[34m";
    if (color == "PINK") return "\033[35m";
    if (color == "ORANGE") return "\033[91m";
    if (color == "ABU") return "\033[37m";

    return "\033[37m";
}


string BoardView::getPlayersOnTile(int pos, const vector<shared_ptr<Player>>& players) {
    stringstream ss;

    for (const auto& p : players) {
        if (p != nullptr && p->getPosition() == pos) {
            ss << p->getUsername() << " ";
        }
    }

    return ss.str();
}

void BoardView::printTop(GameBoard& board, const vector<shared_ptr<Player>>& players) {
    cout << "+";
    for (int i = 20; i <= 30; i++) cout << "--------+";
    cout << endl;

    cout << "|";
    for (int i = 20; i <= 30; i++) {
        cout << setw(8) << formatTile(board.getTileAt(i), players) << "|";
    }
    cout << endl;
}

void BoardView::printBottom(GameBoard& board, const vector<shared_ptr<Player>>& players) {
    cout << "+";
    for (int i = 10; i >= 0; i--) cout << "--------+";
    cout << endl;

    cout << "|";
    for (int i = 10; i >= 0; i--) {
        cout << setw(8) << formatTile(board.getTileAt(i), players) << "|";
    }
    cout << endl;
}

void BoardView::printMiddle(GameBoard& board, const vector<shared_ptr<Player>>& players) {

    int left = 19;
    int right = 31;

    for (int i = 0; i < 9; i++) {

        cout << "|";

        cout << setw(8) << formatTile(board.getTileAt(left--), players);

        cout << "                              "; // space tengah

        cout << setw(8) << formatTile(board.getTileAt(right++), players);

        cout << "|" << endl;
    }
}

void BoardView::printCenterInfo() {

    cout << "              ==========================\n";
    cout << "                     NIMONSOPOLI\n";
    cout << "              ==========================\n\n";

    cout << "                  TURN 15 / 50\n\n";

    cout << "        LEGENDA KEPEMILIKAN & STATUS\n";
    cout << "        P1-P4 : Properti milik pemain\n";
    cout << "        ^  : Rumah level 1\n";
    cout << "        ^^ : Rumah level 2\n";
    cout << "        ^^^: Rumah level 3\n";
    cout << "        *  : Hotel\n\n";

    cout << "        KODE WARNA:\n";
    cout << "        [CK]=Coklat  [MR]=Merah\n";
    cout << "        [BM]=Biru    [KN]=Kuning\n";
}


void BoardView::showBoard(GameBoard& board, const vector<shared_ptr<Player>>& players) {

    printTop(board, players);

    cout << "+--------------------------------------------------------------+\n";

    printMiddle(board, players);

    cout << "+--------------------------------------------------------------+\n";

    printBottom(board, players);

    cout << endl;

    printCenterInfo();
}