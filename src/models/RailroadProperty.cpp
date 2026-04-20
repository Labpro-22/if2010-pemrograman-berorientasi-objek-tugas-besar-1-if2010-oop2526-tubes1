#include "../../include/models/RailroadProperty.hpp"
#include "../../include/models/GameContext.hpp"
#include "../../include/models/Player.hpp"

RailroadProperty::RailroadProperty(const string& code, const string& name,
                                   int purchasePrice, int mortgageValue,
                                   const map<int,int>& rentByCount)
    : Property(code, name, PropertyType::RAILROAD, purchasePrice, mortgageValue), rentByCount(rentByCount) {}

// hitung jumlah railroad, lookup di tabel, kalikan dengan festival multiplier
int RailroadProperty::calculateRent(const GameContext& ctx) const {
    if (!isOwned() || isMortgaged()) return 0;

    int count = 0;
    for (const Player* p : ctx.getAllPlayers()) {
        for (const Property* prop : p->getOwnedProperties()) {
            if (prop->getType() == PropertyType::RAILROAD &&
                prop->getOwner() == owner) {
                count++;
            }
        }
    }

    auto it = rentByCount.find(count);
    if (it == rentByCount.end()) return 0;

    return it->second * festivalMultiplier;
}

bool RailroadProperty::canBuild()    const { return false;}
int  RailroadProperty::getSellValue() const { return purchasePrice;}

const map<int,int>& RailroadProperty::getRentByCount() const {
    return rentByCount;
}