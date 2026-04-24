#pragma once
#include <string>

class GameManager;
class Player;

class Tile
{
protected:
    std::string code;
    std::string name;
    int position;

public:
    Tile(const std::string &code, const std::string &name, int position);
    virtual ~Tile() = default;

    const std::string &getCode() const;
    const std::string &getName() const;
    int getPosition() const;

    virtual void onLanded(GameManager &game, Player &player, int diceTotal) = 0;
    virtual std::string getDisplayType() const = 0;
};
