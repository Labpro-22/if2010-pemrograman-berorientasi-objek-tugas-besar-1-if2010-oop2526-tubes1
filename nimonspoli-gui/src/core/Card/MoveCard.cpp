#include "MoveCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"

class Board;

MoveCard::MoveCard() : SkillCard("Maju " + to_string(rand() % 6 + 1) + " Petak", "MoveCard"), steps(rand() % 6 + 1) {}
MoveCard::MoveCard(int steps) : SkillCard("Maju " + to_string(steps) + " Petak", "MoveCard"), steps(steps) {}

MoveCard::~MoveCard()
{
}

void MoveCard::execute(Player &p, GameState &gs)
{
    int boardSize = gs.getBoard()->getSize();
    int newPos = (p.getPosition() + steps) % boardSize;
    p.setPosition(newPos);
}