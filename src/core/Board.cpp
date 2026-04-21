#include "../../include/core/Board.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "../../include/core/GameManager.hpp"
#include "../../include/core/Tile.hpp"
using namespace std;

Board::Board(int size = 10): tiles(size, nullptr) {}

Tile* Board::getTile(string code) const {
    auto it = std::find_if(tiles.begin(), tiles.end(), [&code](const Tile* tile){
        return tile != nullptr && tile->getCode().compare(code) == 0;
    });

    if (it != tiles.end()) {
        return *it;
    } else {
        return nullptr;
    }
}

Tile* Board::getJailTile() const {
    return getTile("PEN");
}

Tile* Board::goToTile(Tile& current, int moveAmount) const {
    auto it = std::find_if(tiles.begin(), tiles.end(), [&current](const Tile* tile){
        return tile != nullptr && tile->getCode().compare(current.getCode()) == 0;
    });
    if (it != tiles.end()) {
        size_t index = distance(tiles.begin(), it);
        index = (index + moveAmount) % tiles.size();
        return tiles[index];
    } else {
        return nullptr;
    }
}

std::vector<Tile*> Board::getColorGroup(const std::string& color) const {
    std::vector<Tile*> result;
    std::copy_if(tiles.begin(), tiles.end(), std::back_inserter(result), [&color](const Tile* tile){
        return tile != nullptr && tile->getColor().compare(color) == 0;
    });

    return result;
}

void Board::addTile(Tile* newTile){
    tiles.push_back(newTile);
}