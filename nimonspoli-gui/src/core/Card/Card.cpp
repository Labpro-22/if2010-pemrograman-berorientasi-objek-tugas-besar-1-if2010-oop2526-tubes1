#include "Card.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"

Card::Card()
{
}

Card::Card(const string &type, const string &description) : type(type), description(description)
{
}

Card::~Card()
{
}

void Card::execute(Player &p, GameState &gs)
{
}

string Card::getDescription()
{
    return description;
}

string Card::getType()
{
    return type;
}