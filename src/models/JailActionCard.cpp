#include "JailActionCard.hpp"
#include "Tile.hpp"
#include "Board.hpp"
#include "Player.hpp"

std::random_device JailActionCard::rd;
std::mt19937 JailActionCard::gen(JailActionCard::rd());

JailActionCard::JailActionCard(){
    std::uniform_int_distribution<> sentence_dist(1, 4);
    sentenceLength = sentence_dist(gen);
}

void JailActionCard::onDraw(Player& p, Board& b, std::vector<Player>& all){
    int current_pos = p.getPosition();
    
    int total_tiles = b.getSize(); 
    for (int i = 1; i <= total_tiles; ++i) {
        int check_index = (current_pos + i) % total_tiles;
        Tile& ref = b.getTile(check_index);
        GoToJailTile* jail = dynamic_cast<GoToJailTile*>(&ref);
        if (jail != nullptr) {
            p.movePlayer(i);
            jail->onLand(p);
            
            return;
        }
    }
}