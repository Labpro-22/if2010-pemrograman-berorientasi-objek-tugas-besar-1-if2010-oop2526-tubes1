#pragma once

#include <string>
#include <vector>
#include <map>

class StreetConfigData {
public:
    int id;
    std::string code;
    std::string name;
    std::string color;
    float buy_price;
    float mortgage_price;
    float house_price;
    float hotel_price;
    std::vector<int> rent_prices;
};

class ActionTileConfigData {
public:
    int id;
    std::string code;
    std::string name;
    std::string type;
    std::string color;
};

class FullConfigData {
public:
    std::vector<std::unique_ptr<Tile>> streets;
    std::vector<std::unique_ptr<Tile>> action_tiles;
    std::map<int, int> railroad_rent_map;
    std::map<int, int> utility_multiplier_map; 

    int pph_flat;
    int pph_percent;
    int pbm_flat;

    int go_salary;
    int jail_fine;

    int max_turn;
    int initial_balance;

    FullConfigData() : pph_flat(0), pph_percent(0), pbm_flat(0), go_salary(0), jail_fine(0), max_turn(0), initial_balance(0) {}
    FullConfigData(std::vector<std::unique_ptr<Tile>> streets, std::vector<std::unique_ptr<Tile>> action_tiles, std::map<int, int> railroad_rent_map, std::map<int, int> utility_multiplier_map,
                    int pph_flat,int pph_percent,int pbm_flat,int go_salary,int jail_fine,int max_turn,int initial_balance)
                     : streets(std::move(streets)), action_tiles(std::move(action_tiles)), railroad_rent_map(railroad_rent_map), utility_multiplier_map(utility_multiplier_map),
                       pph_flat(pph_flat), pph_percent(pph_percent), pbm_flat(pbm_flat), go_salary(go_salary), jail_fine(jail_fine), max_turn(max_turn), initial_balance(initial_balance)  {}
};