#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "../../include/models/Saveable.hpp"

class Tile;
class Player;

class Board : public Saveable {
private:
    std::vector<std::unique_ptr<Tile>> tiles;
    std::unordered_map<std::string, std::vector<Tile*>> colorTileMap;

public:
    Board();
    std::string toSaveFormat() const;
    Tile& getTile(int index);
    void addTile(std::unique_ptr<Tile> tile);
    const std::vector<Tile*>& getPropertiesByGroup(const std::string& color) const;
    std::vector<std::string> getAllGroups() const;
    bool checkMonopoly(const Player& player, const std::string& color) const;
    int getSize();

};