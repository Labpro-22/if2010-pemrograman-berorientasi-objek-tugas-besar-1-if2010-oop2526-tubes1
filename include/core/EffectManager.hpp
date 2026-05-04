#pragma once

#include <string>
#include <unordered_map>

class Player;
class Property;
class GameEngine;

class EffectManager {
private:
    std::unordered_map<std::string, int> discountDurationByPlayer;

public:
    void onTurnStart(Player& player, GameEngine& game);
    void onTurnEnd(Player& player, GameEngine& game);

    void decrementTemporaryEffects(Player& player);
    void applyFestival(Player& player, Property& property);

    void resetShield(Player& player);

    void applyDiscount(Player& player, int discountPercent);
    void decrementDiscount(Player& player);
};
