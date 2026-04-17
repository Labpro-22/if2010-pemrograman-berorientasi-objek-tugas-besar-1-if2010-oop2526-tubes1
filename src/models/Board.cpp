#include "models/Board.hpp"


Board::Board(const vector<Tile *> &Tile, int Size): tile(Tile), size(Size){}
Tile* Board::getTile(int idx){
    if (idx < 0 || idx >= size)
        return nullptr;
    return tile[idx];
}
Tile* Board::getNextTile(int cur, int next)
{
    int nextIdx = (cur + step) % size;
    return tile[nextIdx];
}
int Board::getSize(){return size;}

Tile::Tile(int id, string display, TileType type, TileName name, string code):id(id), colorDisplay(display), type(type), name(name), code(code){}
int Tile::getIndex(){return id;}
string Tile::getCode(){return code;}
string Tile::getTileName(){return to_string(name);}
string Tile::getTileType(){return to_string(type);}
string Tile::getDisp() { return colorDisplay; }


ActionTile::ActionTile(int id, string display, TileType type, tileName name, string code):Tile(id, display, type, name, code){}

PropertyTile::PropertyTile(int id, string display, TileType type, tileName name, string code, *Propery prop) : Tile(id, display, type, name, code), prop(prop){}
Property* PropertyTile::getProperty(){return prop;}
virtual int calculateRent(int diceTotal) = 0;

StreetTile::StreetTile(int id, string display, TileType type, tileName name, string code) : PropertyTile(id, display, type, name, code) {}
int StreetTile::calculateRent(int diceTotal) override{
    if (!prop)
        return 0;
    return dynamic_cast<StreetProperty *>(prop)->calculateRentPrice(false);
}

RailRoadTile::RailroadTile(int id, string display, TileType type, tileName name, string code) : PropertyTile(id, display, type, name, code) {}
int calculateRent(int diceTotal) override{
    return dynamic_cast<RailroadProperty *>(prop)->calculateRentPrice();
}

UtilityTile::UtilityTile(int id, string display, TileType type, tileName name, string code) PropertyTile(id, display, type, name, code) {}
int UtilityTile::calculateRent(int diceTotal) override{
    return dynamic_cast<UtilityProperty *>(prop)->calculateRentPrice();
}

GoTile::GoTile(int id, string display, TileType type, tileName name, string code, int salary) : ActionTile(id, display, type, name, code), salary(salary){}
void GoTile::onLanded(Player& p, GameState& gs) override{
    p + salary;
}
void GoTile::onPassed(Player& p, GameState& gs){
    p + salary;
}
int GoTile::getSalary(){
    return salary;
}

JailTile::JailTile(int id, string display, TileType type, tileName name, string code, vector<Player *> inmates,
                   vector<Player *> visitor,
                   int jailFine) : ActionTile(id, display, type, name, code), inmates(inmates), visitor(visitor), jailFine(jailFine){}
int JailTile::getJailFine(){return jailFine;}
void JailTile::onLanded(Player& p, GameState& gs) override{
    visitor.push_back(p);
}
void JailTile::sendToJail(Player& p){
    p.setStatus("JAILED");
    inmates.push_back(p);
}
bool JailTile::tryEscape(Player& p, Dice& d){
    int temp = d.getConsecutiveDoubles();
    d.rollRandom();
    if (temp < d.getConsecutiveDoubles()){
        release(p);
        return true;
    }
    return false;
}

void JailTile::payFine(Player& p, Bank& b){
    p - jailFine;
    b.receiveFromPlayer(p, jailFine);
    release(p);
}
void JailTile::useJailCard(Player& p){
    // Kartu bebas penjara ada ketidaksamaan di spek. Pada JailTile dibilang ada tetapi saat di spek card tidak ada jenis tsb
}
void JailTile::release(Player& p){
    p.setStatus("ACTIVE");
    inmates.erase(std::remove(inmates.begin(), inmates.end(), &p), inmates.end());
}
bool JailTile::isInmate(Player& p){
    for (auto inmate : inmates)
    {
        if (inmate->getID() == p.getID())
            return true;
    }
    return false;   
}

GoToJail::GoToJail(int id, string display, TileType type, tileName name, string code) : ActionTile(id, display, type, name, code) {}
void GoToJail::onLanded(Player& p, GameState& gs) override{
    Board* board = gs.getGameBoard();
    JailTile* jail = dynamic_cast<JailTile*>(board->getTile(11)); //Biasanya indeks 11 pada papan monopoli angka tsb bisa menyesuaikan
    if (jail != nullptr)
    {
        p.move(11 - p.getIndex()); 

        jail->sendToJail(p);

        p.setStatus("JAILED");
    }
}

FreeParkingTile::FreeParkingTile(int id, string display, TileType type, tileName name, string code): ActionTile(id, display, type, name, code){}
void FreeParkingTile::onLanded(Player& p, GameState& gs) override{}

TaxTile::TaxTile(int id, string display, TileType type, tileName name, string code) : ActionTile(id, display, type, name, code) {}
void TaxTile::onLanded(Player& p, GameState& gs) override{

    BayarPajakCommand cmdPajak(p, this, gs.getGameBank());
    cmdPajak.execute();
}

CardTile::CardTile(int id, string display, TileType type, tileName name, string code, CardDeck* cardDeck) : ActionTile(id, display, type, name, code), card(cardDeck){}
void CardTile::onLanded(Player& p, GameState& gs) override{
    Card kartu = card->draw();
    kartu.execute(p);
}

FestivalTile::FestivalTile(int id, string display, TileType type, tileName name, string code) : ActionTile(id, display, type, name, code) {}
void FestivalTile::onLanded(Player &, GameState &) override{

}