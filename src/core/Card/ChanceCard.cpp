#include "ChanceCard.hpp"

ChanceCard::ChanceCard()
{
}

ChanceCard::ChanceCard(const string &type, const string &description) : ChanceCard(type, description)
{
}

ChanceCard::~ChanceCard()
{
}

void ChanceCard::execute(Player &p, GameMaster &g)
{
}