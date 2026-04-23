#include "GoToJailCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"

#define JAIL_INDEX 13

GoToJailCard::GoToJailCard() : ChanceCard("Masuk Penjara")
{
}

GoToJailCard::~GoToJailCard()
{
}

void GoToJailCard::execute(Player &p, GameState &gs)
{
    p.setPosition(JAIL_INDEX);
    p.goToJail();
}