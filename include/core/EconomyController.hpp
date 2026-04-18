#pragma once
#include "StreetTile.hpp"
#include "Dice.hpp"
#include "Player.hpp"
#include <map>

using namespace std;
class EconomyController

{
private:
    int goSalary;
    int pph;
    int pbm;
    int jailFine;
    map<int, int> railroadRent; // key: jumlah railroad yang dimiliki, value: rent
    map<int, int> UtilityMultiplier; // key: jumlah utility yang dimiliki, value: multiplier
public:
    int calculateRent(PropertyTile p, Dice d);
    void processTax(Player& p);
    bool buildProperty(Player& p, StreetTile& tile);
    
    void addRailroadRent(int totalOwned, int rent);
    void addUtilityMultiplier(int totalOwned, int multiplier);
    void setGoSalary(int amount);
    void setPph(int amount);
    void setPbm(int amount);
    void setJailFine(int amount);
};
