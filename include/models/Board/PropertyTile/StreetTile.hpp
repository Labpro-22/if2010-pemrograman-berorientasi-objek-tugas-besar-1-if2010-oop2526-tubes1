#pragma once
#include "PropertyTile.hpp"

class StreetTile : public PropertyTile
{
private:
    // string colorGroup;  // warna harusnya ga perlu sudah turunan dari kelas Tile
    int houseCost;
    int hotelCost;
    int houseCount;
    bool hasHotel;
    vector<int> rentPrices; 

public:
    StreetTile(int idx, string code, string name, int price, 
        int morgageValue, vector<int> rentPrices, 
        int houseCost, int hotelCost);
    
    void land(Player &p) override;
};

