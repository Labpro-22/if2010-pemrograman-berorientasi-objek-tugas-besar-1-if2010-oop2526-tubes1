#pragma once
#include "../../include/models/Tile.hpp"
#include <memory>
#include <map>

enum PropertyStatus {
    BANK,
    OWNED,
    MORTGAGED
};

enum class PropertyType {
    STREET,
    RAILROAD,
    UTILITY
};

class PropertyTile : public Tile {
protected:
    float buy_price;
    float mortgage_price;
    std::weak_ptr<Player> owner;
    int festival_level;
    int festival_turns_left;
    PropertyStatus property_status;
    PropertyType type;

public:
    PropertyTile(int index, std::string name, std::string code, std::string color, float buy_price, float mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status);
    virtual ~PropertyTile() = default;

    void applyFestival();
    void decreaseFestival();
;
    float getMortgageValue() const;
    float getBuyPrice() const;
    PropertyStatus getPropertyStatus() const;
    void setPropertyStatus(PropertyStatus ps);
    std::weak_ptr<Player> getPropertyOwner() const;
    void setPropertyOwner(std::shared_ptr<Player> p);
    
    void onLand(Player& p) override;
    virtual float calculateRent() const = 0;
    virtual PropertyType getPropertyType() const = 0;
};

class RailroadTile : public PropertyTile {
private:
    static const std::map<int, int> railroad_multiplier;
public:
    RailroadTile(int index, std::string name, std::string code, std::string color, float buy_price, float mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status);
    float calculateRent() const override;
    void onLand(Player& p) override;
    PropertyType getPropertyType() const override;
};

class UtilityTile : public PropertyTile {
private:
    static const std::map<int, int> utility_multiplier;
public:
    UtilityTile(int index, std::string name, std::string code, std::string color, float buy_price, float mortgage_price, std::shared_ptr<Player> owner, int festival_level, int festival_turns_left, PropertyStatus property_status);
    float calculateRent() const override;
    void onLand(Player& p) override;  
    PropertyType getPropertyType() const override;
};
