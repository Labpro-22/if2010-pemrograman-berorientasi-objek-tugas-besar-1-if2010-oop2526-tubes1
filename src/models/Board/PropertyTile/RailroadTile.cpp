#include "RailroadTile.hpp"
RailroadTile::RailroadTile(int idx, string code, string name, int price, int morgageValue, vector<int> rentPrices) : PropertyTile(idx, code, name, price, morgageValue, rentPrices) {
    this->PropertyType = RAILROAD;
}

void RailroadTile::land(Player &p) {
    
}
