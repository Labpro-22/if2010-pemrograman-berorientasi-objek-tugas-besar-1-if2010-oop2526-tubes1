#include "../../include/models/UtilityProperty.hpp"
#include "../../include/models/GameContext.hpp"
#include "../../include/models/Player.hpp"

UtilityProperty::UtilityProperty(const string& code, const string& name,
                                 int purchasePrice, int mortgageValue, const std::map<int,int>& multiplierByCount)
    : Property(code, name, PropertyType::UTILITY, purchasePrice, mortgageValue), multiplierByCount(multiplierByCount) {}

int UtilityProperty::calculateRent(const GameContext& ctx) const {
    if (!isOwned() || isMortgaged()) return 0;

    int count = 0;
    for (const Player* p : ctx.getAllPlayers()) {
        for (const Property* prop : p->getOwnedProperties()) {
            if (prop->getType() == PropertyType::UTILITY &&
                prop->getOwner() == owner) {
                count++;
            }
        }
    }

    auto it = multiplierByCount.find(count);
    if (it == multiplierByCount.end()) return 0;

    return ctx.getDiceTotal() * it->second * festivalMultiplier;
}

bool UtilityProperty::canBuild()    const { return false;          }
int  UtilityProperty::getSellValue() const { return purchasePrice; }

const map<int,int>& UtilityProperty::getMultiplierByCount() const {
    return multiplierByCount;
}