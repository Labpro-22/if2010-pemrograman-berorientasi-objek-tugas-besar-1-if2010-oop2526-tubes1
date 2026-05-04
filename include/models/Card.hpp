#pragma once

#include <string>

class Player;
class GameEngine;

class Card {
protected:
    std::string description;

public:
    explicit Card(const std::string& description);
    virtual ~Card() = default;

    virtual void apply(Player& player, GameEngine& game) = 0;

    std::string getDescription() const;
};
