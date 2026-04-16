#pragma once
#include "PropertyTile.hpp"
#include <vector>

class StreetTile : public PropertyTile {
private:
    int house_count;
    std::vector<int> rent_list;
    int house_price;
    int hotel_price;

public:
    StreetTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status, int house_count, std::vector<int> rent_list, int house_price, int hotel_price);
    int calculateRent() const override;
    void onLand(Player& p) override;
    bool canBuildHouse() const override;
    bool canBuildHotel() const override;
    void buildHouse() override;
    void buildHotel() override;
};