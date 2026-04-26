#include "utils/PropertyTile.hpp"
#include "models/Player.hpp"
#include <sstream>
#include <algorithm>

PropertyTile::PropertyTile(int id, std::string code, std::string name, int mortgageValue)
: Tile(id, code, name),
      owner(nullptr),
      status(BANK),
      mortgageValue(mortgageValue),
      festivalMultiplier(1),
      festivalDuration(0) {}

void PropertyTile::onLand(Player* player, Game* game) {
    if (player == nullptr || status != OWNED || owner == nullptr || owner == player) {
        return;
    }

    if (player->isShieldActive()) {
        return;
    }

    int rent = calculateRent(player, game);

    int discount = std::clamp(player->getDiscountPercent(), 0, 100);
    rent = rent * (100 - discount) / 100;

    if (rent > 0) {
        player->pay(rent);
        owner->receive(rent);
    }
}

void PropertyTile::setOwner(Player* owner) {
    this->owner = owner;
    status = (owner == nullptr ? BANK : OWNED);
}

Player* PropertyTile::getOwner() const { return owner; }
PropertyStatus PropertyTile::getStatus() const { return status; }
int PropertyTile::getMortgageValue() const { return mortgageValue; }

int PropertyTile::mortgage() {
    status = MORTGAGED;
    return mortgageValue;
}

int PropertyTile::redeem() {
    status = OWNED;
    return getLandPrice();
}

bool PropertyTile::isOwned() const { return status == OWNED; }
bool PropertyTile::isMortgaged() const { return status == MORTGAGED; }

void PropertyTile::applyFestival() {
    if (festivalMultiplier < 8)
        festivalMultiplier *= 2;
    festivalDuration = 3;
}

void PropertyTile::decrementFestivalDuration() {
    if (festivalDuration > 0)
        festivalDuration--;
}

void PropertyTile::resetFestivalIfExpired() {
    if (festivalDuration == 0)
        festivalMultiplier = 1;
}

int PropertyTile::getFestivalMultiplier() const { return festivalMultiplier; }
int PropertyTile::getFestivalDuration() const { return festivalDuration; }

std::string PropertyTile::toString() const {
    std::ostringstream oss;
    oss << name << " (" << code << ")"
        << " | status=" << (status == BANK ? "BANK" : status == OWNED ? "OWNED" : "MORTGAGED")
        << " | festival=x" << festivalMultiplier
        << " dur=" << festivalDuration;
    return oss.str();
}

bool PropertyTile::canBeMortgaged(Game* game) const {
    (void) game;
    return status == OWNED;
}

bool PropertyTile::isOwnable() const { return true; }