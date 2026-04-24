#include "models/base/Tile.hpp"

Tile::Tile(const std::string &code, const std::string &name, int position)
    : code(code), name(name), position(position) {}

const std::string &Tile::getCode() const
{
    return code;
}

const std::string &Tile::getName() const
{
    return name;
}

int Tile::getPosition() const
{
    return position;
}
