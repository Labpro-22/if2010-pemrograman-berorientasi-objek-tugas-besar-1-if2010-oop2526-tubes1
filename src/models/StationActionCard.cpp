#include "StationActionCard.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "PropertyTile.hpp"

void StationActionCard::onDraw(Player& p, Board& b, std::vector<Player>& all) {
    int current_pos = p.getPosition();
    
    int total_tiles = b.getSize(); 
    for (int i = 1; i <= total_tiles; ++i) {
        int check_index = (current_pos + i) % total_tiles;
        Tile& ref = b.getTile(check_index);
        RailroadTile* station = dynamic_cast<RailroadTile*>(&ref);
        if (station != nullptr) {
            p.movePlayer(i);
            station->onLand(p);
            
            return;
        }
    }
}