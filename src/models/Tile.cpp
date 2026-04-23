#include "Tile.hpp"
#include "TileVisitor.hpp"

// Tile
Tile::Tile(int index, std::string name, std::string code, std::string color) 
    : index(index), name(name), code(code), color(color) {}

int Tile::getIndex() const {
    return index;
}

std::string Tile::getName() const {
    return name;
}

std::string Tile::getCode() const {
    return code;
}

// CardTile 
CardTile::CardTile(int index, std::string name, std::string code, std::string color, CardType card) 
    : Tile(index, name, code, color), card(card) {}

void CardTile::onLand(Player& p, TileVisitor& visitor) {
    visitor.visitCardTile(this, p);
}

// TaxTile
TaxTile::TaxTile(int index, std::string name, std::string code, std::string color, TaxType tax_type, int tax_amount)
    :  Tile(index, name, code, color), tax_type(tax_type), tax_amount(tax_amount) {}

void TaxTile::onLand(Player& p, TileVisitor& visitor) {
    visitor.visitTaxTile(this, p);
}

// FestivalTile
FestivalTile::FestivalTile(int index, std::string name, std::string code, std::string color)
    : Tile(index, name, code, color) {}

void FestivalTile::onLand(Player& p, TileVisitor& visitor){
    visitor.visitFestivalTile(this, p);
}

// GoTile
GoTile::GoTile(int index, std::string name, std::string code, std::string color, int go_reward) 
    : Tile(index, name, code, color), go_reward(go_reward) {}

int GoTile::getReward(){
    return go_reward;
}

void GoTile::onLand(Player& p, TileVisitor& visitor) {
    visitor.visitGoTile(this, p);
}

// GoToJailTile
GoToJailTile::GoToJailTile(int index, std::string name, std::string code, std::string color)
    : Tile(index, name, code, color) {}

void GoToJailTile::onLand(Player& p, TileVisitor& visitor) {
    visitor.visitGoToJailTile(this, p);
}

// FreeParkingTile
FreeParkingTile::FreeParkingTile(int index, std::string name, std::string code, std::string color)
    : Tile(index, name, code, color) {}

void FreeParkingTile::onLand(Player& p, TileVisitor& visitor) {
    visitor.visitFreeParkingTile(this, p);
}

// JailTile
JailTile::JailTile(int index, std::string name, std::string code, std::string color, int jail_fine)
    : Tile(index, name, code, color), jail_fine(jail_fine) {}

void JailTile::onLand(Player& p, TileVisitor& visitor) {
    visitor.visitJailTile(this, p);
}