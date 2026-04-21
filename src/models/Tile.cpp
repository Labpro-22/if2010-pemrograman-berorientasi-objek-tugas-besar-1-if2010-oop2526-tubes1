#include "../include/models/Tile.hpp"

Tile::Tile(int position, const std::string& name, const std::string& color)
    : position(position), name(name), color(color) {
}

int Tile::getPosition() const {
    return position;
}

std::string Tile::getName() const {
    return name;
}

std::string Tile::getColor() const {
    return color;
}
