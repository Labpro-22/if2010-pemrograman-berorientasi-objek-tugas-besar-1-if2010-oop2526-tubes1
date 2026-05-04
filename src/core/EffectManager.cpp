#include "../../include/core/EffectManager.hpp"

#include "../../include/core/GameEngine.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Property.hpp"
#include "../../include/utils/GameException.hpp"

void EffectManager::onTurnStart(Player& player, GameEngine& game) {
    (void)game;
    player.setUsedSkillThisTurn(false);
    player.resetConsecutiveDoubles();
}

void EffectManager::onTurnEnd(Player& player, GameEngine& game) {
    (void)game;
    decrementTemporaryEffects(player);
    decrementDiscount(player);
    resetShield(player);
}

void EffectManager::decrementTemporaryEffects(Player& player) {
    const auto& properties = player.getOwnedProperties();
    for (Property* property : properties) {
        if (!property) {
            continue;
        }

        const int dur = property->getFestivalDuration();
        if (dur <= 0) {
            continue;
        }

        property->setFestivalDuration(dur - 1);
        if (property->getFestivalDuration() <= 0) {
            property->setFestivalDuration(0);
            property->setFestivalMultiplier(1);
        }
    }
}

void EffectManager::applyFestival(Player& player, Property& property) {
    if (property.getOwner() != &player) {
        throw NotOwnerException(player.getUsername(), property.getCode());
    }

    int multiplier = property.getFestivalMultiplier();
    if (multiplier < 1) {
        multiplier = 1;
    }

    if (multiplier < 8) {
        multiplier *= 2;
    }

    property.setFestivalMultiplier(multiplier);
    property.setFestivalDuration(3);
}

void EffectManager::resetShield(Player& player) {
    player.setShieldActive(false);
}

void EffectManager::applyDiscount(Player& player, int discountPercent) {
    player.setDiscountPercent(discountPercent);
    discountDurationByPlayer[player.getUsername()] = 1;
}

void EffectManager::decrementDiscount(Player& player) {
    auto it = discountDurationByPlayer.find(player.getUsername());
    if (it == discountDurationByPlayer.end()) {
        player.setDiscountPercent(0);
        return;
    }

    it->second -= 1;
    if (it->second <= 0) {
        discountDurationByPlayer.erase(it);
        player.setDiscountPercent(0);
    }
}
