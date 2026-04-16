#pragma once
#include "Tile.hpp"
#include <memory>

enum PropertyStatus {
    BANK,
    OWNED,
    MORTGAGED
};

class PropertyTile : public Tile {
protected:
    int buy_price;
    int mortgage_price;
    std::weak_ptr<Player> owner;
    int festival_level;
    int festival_turns_left;
    PropertyStatus property_status;

public:
    PropertyTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status);
    virtual ~PropertyTile() = default;
    void onLand(Player& p) override;
    void applyFestival();
    void decreaseFestival();
    int getSellValue() const;
    int getMortgageValue() const;
    PropertyStatus getPropertyStatus() const;
    void setPropertyStatus(PropertyStatus ps);
    std::weak_ptr<Player> getPropertyOwner() const;
    void setPropertyOwner(std::shared_ptr<Player> p);
    virtual int calculateRent() const = 0;
    virtual bool canBuildHouse() const = 0;
    virtual bool canBuildHotel() const = 0;
    virtual void buildHouse() = 0;
    virtual void buildHotel() = 0;
};

class RailroadTile : public PropertyTile {
public:
    RailroadTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status);
    int calculateRent() const override;
    void onLand(Player& p) override;
};

class UtilityTile : public PropertyTile {
private:
    int multiplier_factor;

public:
    UtilityTile(int index, std::string name, std::string code, std::string color, int buy_price, int mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status, int multiplier_factor);
    int calculateRent() const override;
    void onLand(Player& p) override;
};