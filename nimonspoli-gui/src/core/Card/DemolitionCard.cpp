#include "DemolitionCard.hpp"
#include "../GameState/GameState.hpp"
#include "../Board/Board.hpp"
#include "../GameState/GameState.hpp"
#include "../Player/Player.hpp"
#include "../Property/Property.hpp"
#include "../Property/StreetProperty.hpp"

class GameMaster;
class Board;

DemolitionCard::DemolitionCard() : SkillCard("Hancurkan properti milik lawan", "DemolitionCard")
{
}

DemolitionCard::DemolitionCard(const string &type, const string &description, bool used) : SkillCard(type, description, used), targetPropertyId(-1)
{
}

DemolitionCard::~DemolitionCard()
{
}
int DemolitionCard::getTargetPropertyId() const
{
    return targetPropertyId;
}
void DemolitionCard::setTargetProperty(int targetPropertyId)
{
    this->targetPropertyId = targetPropertyId;
}

void DemolitionCard::execute(Player &p, GameState &gs)
{
    if (targetPropertyId == -1)
        return;

    GameMaster *gm = gs.getGameMaster();
    if (!gm)
        return;

    Board *board = gm->getState().getBoard();
    if (!board)
        return;

    Property *targetProperty = board->findPropertyById(targetPropertyId);
    if (!targetProperty)
        return;

    if (targetProperty->getOwnerId() == p.getUsername())
        return;

    if (targetProperty->getStatus() != PropertyStatus::OWNED)
        return;

    StreetProperty *sp = dynamic_cast<StreetProperty *>(targetProperty);
    if (!sp)
        return;

    if (sp->getBuildingCount() <= 0 && !sp->gethasHotel())
        return;

    sp->sellAllBuildings();

    targetPropertyId = -1;
    markUsed();
}

string DemolitionCard::successMessage() const
{
    return "DemolitionCard digunakan. Pilih properti milik lawan yang ingin dihancurkan.";
}