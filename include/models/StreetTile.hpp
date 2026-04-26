#pragma once
#include "PropertyTile.hpp"
#include <vector>

class StreetTile : public PropertyTile {
private:
    int building_level;
    std::vector<float> rent_list;
    float house_price;
    float hotel_price;

public:
    StreetTile(int index, std::string name, std::string code, std::string color, float buy_price, float mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status, int building_level, const std::vector<float>& rent_list, float house_price, float hotel_price);
               
    float calculateRent() const override;
    void onLand(Player& p) override;
    PropertyType getPropertyType() const override;

    int getBuildingLevel() const;
    bool canBuildHouse() const;
    bool canBuildHotel() const;
    
    bool canSellBuilding() const;

    void buildHouse();
    void buildHotel();
    void sellBuilding();

    float getHousePrice() const;
    float getHotelPrice() const;
};