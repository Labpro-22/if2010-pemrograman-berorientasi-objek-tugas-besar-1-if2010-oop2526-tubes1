#include "GeneralFundCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"

GeneralFundCard::GeneralFundCard()
{
}

GeneralFundCard::GeneralFundCard(const string &type, const string &description) : Card(type, description)
{
}

GeneralFundCard::~GeneralFundCard()
{
}

void GeneralFundCard::execute(Player &p, GameState &gs)
{
}