#include "DemolitionCard.hpp"
#include "../GameState/GameState.hpp"
#include "../Player/Player.hpp"
#include "../Property/Property.hpp"

class Property;

DemolitionCard::DemolitionCard() : SkillCard("Hancurkan properti milik lawan", "DemolitionCard")
{
}

DemolitionCard::~DemolitionCard()
{
}

void DemolitionCard::execute(Player &p, GameState &gs)
{
    vector<Player *> players = gs.getActivePlayers();

    for (Player *other : players)
    {
        if (other == &p)
            continue;
        if (other->getPropertyCount() > 0)
        {
            Property *prop = other->getProperties()[0];
            other->removeProperty(prop);
            prop->clearOwner();
            prop->setStatus(PropertyStatus::BANK);
            break;
        }
    }
}