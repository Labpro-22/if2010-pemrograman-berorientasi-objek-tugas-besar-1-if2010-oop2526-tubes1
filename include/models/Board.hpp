#pragma once
#include <vector>
#include <string>
#include <map>
#include "Tile.hpp"

class Board {
private:
    static Board* instance;
    vector<Tile*> tiles;
    map<std::string, int> tileMap;
    int jailPositionIndex;
    Board();
public:
    friend class ConfigLoader; 
    static Board* getInstance();
    ~Board();
    int getTotalTiles() const;
    void printBoardStatus(); 
    void resetBoard();
    Tile* getTileAt(int index); 
    Tile* getTileByKode(const std::string& kode);
    int getJailPosition() const;
};