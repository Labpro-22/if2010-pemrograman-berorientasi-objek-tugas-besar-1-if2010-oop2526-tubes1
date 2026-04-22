#include "../../include/views/BoardView.hpp"
#include "../../include/models/GameBoard.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/models/Property.hpp"

#include <iostream>
#include <sstream>

using namespace std;

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


string BoardView::getPlayersOnTile(int pos, const std::vector<std::shared_ptr<Player>> players) {
    stringstream ss;

    for (const auto& p : players) {
        if (p != nullptr && p->getPosition() == pos) {
            ss << p->getUsername() << " ";
        }
    }

    return ss.str();
}

string BoardView::renderTile(Tile* tile, const std::vector<std::shared_ptr<Player>> players) {
    string color = getColorCode(tile->getColor());
    string name = tile->getName();
    int pos = tile->getPosition();

    string playersStr = getPlayersOnTile(pos, players);

    stringstream ss;
    ss << color << "[" << name << "]" << "\033[0m";

    if (!playersStr.empty()) {
        ss << " (" << playersStr << ")";
    }

    Property* prop = dynamic_cast<Property*>(tile);
    if (prop != nullptr) {
        string owner = prop->getOwner();
        if (!owner.empty()) {
            ss << " {" << owner << "}";
        }
    }

    return ss.str();
}

void BoardView::showBoard(GameBoard& board, const std::vector<std::shared_ptr<Player>> players) {
    cout << "\n=== MONOPOLI BOARD ===\n";

    for (int i = 0; i < 40; i++) {
        Tile* tile = board.getTileAt(i);
        
        cout << "[" << i << "] " << renderTile(tile, players) << endl;
    }

    cout << "==============\n";
}