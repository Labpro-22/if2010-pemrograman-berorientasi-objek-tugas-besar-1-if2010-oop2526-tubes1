#pragma once
#include "GameBoard.hpp"

class DisplayView
{
private:
    int screenWidth;
    GameBoard* boardMap;
public:
    void renderBoard();
    void renderAkta(StreetTile &Street);
    void renderProperty();
    void showMenu();
};
