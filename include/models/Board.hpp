#pragma once
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "Property.hpp"
using namespace std;
class Board
{
private:
    vector<Tile*> tile;
    int size;
 public : 
    Board(const vector<Tile*>& Tile, int Size);
    Tile* getTile(int idx);
    Tile* getNextTile(int cur, int next);
    int getSize();
};

enum class TileType
{
    SPECIAL,
    STREET,
    CARD,
    TAX,
    RAILROAD,
    FESTIVAL,
    UTILITY
};

enum class TileName
{
    GO,
    GARUT,
    COMMON_FUND,
    TASIKMALAYA,
    INCOME_TAX,
    ST_GAMBIR,
    BOGOR,
    FESTIVAL,
    DEPOK,
    BEKASI,
    JAIL,
    MAGELANG,
    PLN,
    SOLO,
    YOGYAKARTA,
    ST_BANDUNG,
    MALANG,
    SEMARANG,
    SURABAYA,
    FREE_PARK,
    MAKASSAR,
    CHANCE,
    BALIKPAPAN,
    MANADO,
    ST_TUGU,
    PALEMBANG,
    PEKANBARU,
    PAM,
    MEDAN,
    GO_TO_JAIL,
    BANDUNG,
    DENPASAR,
    MATARAM,
    ST_GUBENG,
    JAKARTA,
    LUX_GOODS,
    IKN
};

class Tile{
    protected:
         int id;
         string colorDisplay;
         TileType type;
         TileName name;
         string code;
    public:
        Tile(int id, string display, TileType type, TileName name, string code);
        virtual void onLanded(Player& p, GameState& gs)= 0;
        virtual ~Tile() = default;
        int getIndex();
        string getCode();
        string getTileName();
        string getTileType();
        string getDisp();
};

class ActionTile: public Tile{
    public:
        ActionTile(int id, string display, tileType type, TileName name, string code);
};

class PropertyTile: public Tile{
    protected:
        Property* prop;
    public:
        PropertyTile(int id, string display, TileType type, TileName name, string code);
        Property* getProperty();
        virtual int calculateRent(int diceTotal) = 0;
};

class StreetTile: public PropertyTile{
    public:
        StreetTile(int id, string display, TileType type, TileName name, string code);
        int calculateRent(int diceTotal) override;
};

class RailroadTile: public PropertyTile{
    public:
        RailroadTile(int id, string display, TileType type, TileName name, string code);
        int calculateRent(int diceTotal) override;
};

class UtilityTile: public PropertyTile{
    public:
        UtilityTile(int id, string display, TileType type, TileName name, string code);
        int calculateRent(int diceTotal) override;
};

class GoTile: public ActionTile{
    private:
        int salary;
    public:
        GoTile(int id, string display, TileType type, TileName name, string code, int salary);
        void onLanded(Player&, GameState&) override;
        void onPassed(Player&, GameState&);
        int getSalary();
};

class JailTile: public ActionTile{
    private:
        vector<Player*> inmates;
        vector<Player*> visitor;
        int jailFine;

    public:
        JailTile(int id, string display, TileType type, TileName name, string code,  vector<Player *> inmates,
                 vector<Player *> visitor,
                 int jailFine);
        int getJailFine();
        void onLanded(Player&, GameState&) override;
        void sendToJail(Player&);
        bool tryEscape(Player&, Dice&);
        void payFine(Player&, Bank&);
        void useJailCard(Player&);
        void release(Player&);
        bool isInmate(Player&);
};

class GoToJail: public ActionTile{
    public:
        GoToJail(int id, string display, TileType type, TileName name, string code);
        void onLanded(Player&, GameState&) override;
};

class FreeParkingTile: public ActionTile{
    public:
        FreeParkingTile(int id, string display, TileType type, TileName name, string code);
        void onLanded(Player&, GameState&) override;
};

class TaxTile: public ActionTile{
    public:
        TaxTile(int id, string display, TileType type, TileName name, string code);
        void onLanded(Player&, GameState&) override;
};

class CardTile: public ActionTile{
    private:
        CardDeck* card;
    public:
        CardTile(int id, string display, TileType type, TileName name, string code, CardDeck*);
        void onLanded(Player&, GameState&) override;
};

class FestivalTile: public ActionTile{
    public:
    FestivalTile(int id, string display, TileType type, TileName name, string code);
    void onLanded(Player&, GameState&) override;
};