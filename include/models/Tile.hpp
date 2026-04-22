#pragma once

#include <string>

// Forward declaration
class Player;

class Tile {
protected:
    int position;
    std::string name;
    std::string color;

public:
    Tile(int position, const std::string& name, const std::string& color);
    virtual ~Tile() = default;

    int getPosition() const;
    std::string getName() const;
    std::string getColor() const;

    virtual void onLand(Player* player) = 0;
    virtual void onPass(Player* player) = 0;
};
