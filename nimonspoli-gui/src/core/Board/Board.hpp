#pragma once
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "../Card/CardDeck.hpp"
#include "../Dice/Dice.hpp"
using namespace std;

// Forward declarations
class Player;
class GameState;
class Card;
class Property;
class Bank;

// Enum dideklarasikan lebih awal agar bisa dipakai Tile
enum class TileType { SPECIAL, STREET, CARD, TAX, RAILROAD, FESTIVAL, UTILITY };
enum class TileName {
    GO, GARUT, COMMON_FUND, TASIKMALAYA, INCOME_TAX, ST_GAMBIR, BOGOR,
    FESTIVAL, DEPOK, BEKASI, JAIL, MAGELANG, PLN, SOLO, YOGYAKARTA,
    ST_BANDUNG, MALANG, SEMARANG, SURABAYA, FREE_PARK, MAKASSAR, CHANCE,
    BALIKPAPAN, MANADO, ST_TUGU, PALEMBANG, PEKANBARU, PAM, MEDAN,
    GO_TO_JAIL, BANDUNG, DENPASAR, MATARAM, ST_GUBENG, JAKARTA, LUX_GOODS, IKN
};

string tileTypeToString(TileType type);
string tileNameToString(TileName name);

// Tile dideklarasikan sebelum Board
class Tile {
protected:
    int id;
    string colorDisplay;
    TileType type;
    TileName name;
    string code;
public:
    Tile(int id, string display, TileType type, TileName name, string code);
    virtual void onLanded(Player& p, GameState& gs) = 0;
    virtual ~Tile() = default;
    int getIndex() const;
    string getCode() const;
    string getTileName() const;
    string getTileType() const;
    string getDisp() const;
};

// Board dideklarasikan setelah Tile
class Board {
private:
    vector<Tile*> tile;
    int size;
public:
    Board(const vector<Tile*>& Tile, int Size);
    ~Board() = default;
    Tile* getTile(int idx) const;
    Tile* getNextTile(int cur, int next) const;
    int getSize() const;
    int findTileIndexByCode(const string &code) const;
    Tile* findTileByCode(const string &code) const;
};

class ActionTile: public Tile {
public:
    ActionTile(int id, string display, TileType type, TileName name, string code);
};

class PropertyTile: public Tile {
protected:
    Property* prop;
public:
    PropertyTile(int id, string display, TileType type, TileName name, string code);
    Property* getProperty() const;
    virtual int calculateRent(int diceTotal) const = 0;
};

class StreetTile: public PropertyTile {
public:
    StreetTile(int id, string display, TileType type, TileName name, string code);
    int calculateRent(int diceTotal) const override;
};

class RailroadTile: public PropertyTile {
public:
    RailroadTile(int id, string display, TileType type, TileName name, string code);
    int calculateRent(int diceTotal) const override;
};

class UtilityTile: public PropertyTile {
public:
    UtilityTile(int id, string display, TileType type, TileName name, string code);
    int calculateRent(int diceTotal) const override;
};

class GoTile: public ActionTile {
private:
    int salary;
public:
    GoTile(int id, string display, TileType type, TileName name, string code, int salary);
    void onLanded(Player&, GameState&) override;
    void onPassed(Player&, GameState&);
    int getSalary() const;
};

class JailTile: public ActionTile {
private:
    vector<Player*> inmates;
    vector<Player*> visitor;
    int jailFine;
public:
    JailTile(int id, string display, TileType type, TileName name, string code,
             vector<Player*> inmates, vector<Player*> visitor, int jailFine);
    int getJailFine() const;
    void onLanded(Player&, GameState&) override;
    void sendToJail(Player&);
    bool tryEscape(Player&, Dice&);
    void payFine(Player&, Bank&);
    void useJailCard(Player&);
    void release(Player&);
    bool isInmate(Player&) const;
};

class GoToJail: public ActionTile {
public:
    GoToJail(int id, string display, TileType type, TileName name, string code);
    void onLanded(Player&, GameState&) override;
};

class FreeParkingTile: public ActionTile {
public:
    FreeParkingTile(int id, string display, TileType type, TileName name, string code);
    void onLanded(Player&, GameState&) override;
};

class TaxTile: public ActionTile {
public:
    TaxTile(int id, string display, TileType type, TileName name, string code);
    void onLanded(Player&, GameState&) override;
};

class CardTile: public ActionTile {
private:
    CardDeck<Card>* card;  // Perbaikan: tambah template argument
public:
    CardTile(int id, string display, TileType type, TileName name, string code, CardDeck<Card>*);
    void onLanded(Player&, GameState&) override;
};

class FestivalTile: public ActionTile {
public:
    FestivalTile(int id, string display, TileType type, TileName name, string code);
    void onLanded(Player&, GameState&) override;
};