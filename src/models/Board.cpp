#include "../../include/models/Board.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/models/PropertyTile.hpp"
#include "../../include/models/Player.hpp"
#include <stdexcept>

Board::Board() = default;

std::string Board::toSaveFormat() const {
    
}

Tile& Board::getTile(int index) {
    if (tiles.empty()) {
        throw std::runtime_error("Cannot get tile: Board is empty!");
    }
    int safe_index = index % tiles.size();
    return *tiles[safe_index];
}

void Board::addTile(std::unique_ptr<Tile> tile) {
    Tile* raw_ptr = tile.get();
    std::string color = raw_ptr->getColor();
    if (!color.empty() && color != "None") {
        colorTileMap[color].push_back(raw_ptr);
    }

    tiles.push_back(std::move(tile));
}

const std::vector<Tile*>& Board::getPropertiesByGroup(const std::string& color) const {
    auto it = colorTileMap.find(color);
    
    if (it != colorTileMap.end()) {
        return it->second;
    }

    static const std::vector<Tile*> empty_vector;
    return empty_vector;
}

std::vector<std::string> Board::getAllGroups() const {
    std::vector<std::string> groups;

    for (const auto& pair : colorTileMap) {
        groups.push_back(pair.first);
    }
    
    return groups;
}


bool Board::checkMonopoly(const Player& player, const std::string& color) const {
    auto it = colorTileMap.find(color);

    if (it == colorTileMap.end() || it->second.empty()) {
        return false;
    }
    for (Tile* tile : it->second) {
        PropertyTile* prop = dynamic_cast<PropertyTile*>(tile);
        
        if (prop != nullptr) {
            std::shared_ptr<Player> owner = prop->getPropertyOwner().lock();
            if (!owner || owner.get() != &player) {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

int Board::getSize() {
    return static_cast<int>(tiles.size());
}