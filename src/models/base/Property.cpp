#include "models/base/Property.hpp"
#include "core/game_manager/GameManager.hpp"
#include "models/player/Player.hpp"

Property::Property(const std::string &code,
                   const std::string &name,
                   int position,
                   PropertyType propertyType,
                   int purchasePrice,
                   int mortgageValue)
    : Tile(code, name, position),
      propertyType(propertyType),
      status(PropertyStatus::BANK),
      owner(0),
      purchasePrice(purchasePrice),
      mortgageValue(mortgageValue),
      festivalMultiplierSteps(0),
      festivalTurnsRemaining(0) {}

PropertyType Property::getPropertyType() const { return propertyType; }
PropertyStatus Property::getStatus() const { return status; }
Player *Property::getOwner() const { return owner; }
int Property::getPurchasePrice() const { return purchasePrice; }
int Property::getMortgageValue() const { return mortgageValue; }

bool Property::isOwned() const { return status == PropertyStatus::OWNED && owner != 0; }
bool Property::isMortgaged() const { return status == PropertyStatus::MORTGAGED; }

void Property::setOwner(Player *newOwner)
{
    owner = newOwner;
    status = (newOwner == 0 ? PropertyStatus::BANK : PropertyStatus::OWNED);
}

void Property::clearOwner()
{
    owner = 0;
    status = PropertyStatus::BANK;
    clearFestival();
}

void Property::setStatus(PropertyStatus newStatus) { status = newStatus; }
void Property::mortgage() { status = PropertyStatus::MORTGAGED; }
void Property::unmortgage()
{
    if (owner != 0)
        status = PropertyStatus::OWNED;
}

int Property::getFestivalMultiplierSteps() const { return festivalMultiplierSteps; }
int Property::getFestivalTurnsRemaining() const { return festivalTurnsRemaining; }
int Property::getFestivalMultiplier() const
{
    int multiplier = 1;
    for (int i = 0; i < festivalMultiplierSteps; ++i)
    {
        multiplier *= 2;
    }
    return multiplier;
}

void Property::applyFestivalBoost()
{
    if (festivalMultiplierSteps < 3)
    {
        ++festivalMultiplierSteps;
    }
    festivalTurnsRemaining = 3;
}

void Property::restoreFestivalState(int multiplierSteps, int turnsRemaining)
{
    if (multiplierSteps < 0)
        multiplierSteps = 0;
    if (multiplierSteps > 3)
        multiplierSteps = 3;
    if (turnsRemaining < 0)
        turnsRemaining = 0;
    festivalMultiplierSteps = multiplierSteps;
    festivalTurnsRemaining = turnsRemaining;
}

void Property::tickFestival()
{
    if (festivalTurnsRemaining > 0)
    {
        --festivalTurnsRemaining;
        if (festivalTurnsRemaining == 0)
        {
            festivalMultiplierSteps = 0;
        }
    }
}

void Property::clearFestival()
{
    festivalMultiplierSteps = 0;
    festivalTurnsRemaining = 0;
}

void Property::onLanded(GameManager &game, Player &player, int diceTotal)
{
    game.handlePropertyLanding(player, *this, diceTotal);
}

std::string Property::getDisplayType() const
{
    return "PROPERTY";
}
