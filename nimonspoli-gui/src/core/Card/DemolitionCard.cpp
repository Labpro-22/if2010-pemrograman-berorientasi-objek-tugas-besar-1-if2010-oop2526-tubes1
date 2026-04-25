#include "DemolitionCard.hpp"
#include "../GameState/GameState.hpp"
#include "../Player/Player.hpp"
#include "../Property/Property.hpp"
#include "../Property/StreetProperty.hpp"

DemolitionCard::DemolitionCard() : SkillCard("Hancurkan properti milik lawan", "DemolitionCard")
{
}

DemolitionCard::DemolitionCard(const string &type, const string &description, bool used) : SkillCard(type, description, used)
{
}

DemolitionCard::~DemolitionCard()
{
}

void DemolitionCard::setTargetProperty(Property *target)
{
    targetProperty = target;
}

void DemolitionCard::execute(Player &p, GameState &gs)
{
    if (targetProperty)
    {
        StreetProperty *sp = dynamic_cast<StreetProperty *>(targetProperty);
        if (sp && (sp->getBuildingCount() > 0 || sp->gethasHotel()))
        {
            sp->sellAllBuildings();
        }
    }
    else
    {
        vector<Player *> players = gs.getActivePlayers();

        for (Player *other : players)
        {
            if (other == &p)
                continue;
            vector<Property *> props = other->getProperties();
            for (Property *prop : props)
            {
                StreetProperty *sp = dynamic_cast<StreetProperty *>(prop);
                if (sp && (sp->getBuildingCount() > 0 || sp->gethasHotel()))
                {
                    sp->sellAllBuildings();
                    break;
                }
            }
            break;
        }
    }
    
    targetProperty = nullptr;
    markUsed();
}

string DemolitionCard::successMessage() const
{
    return "DemolitionCard digunakan. Pilih properti milik lawan yang ingin dihancurkan.";
}