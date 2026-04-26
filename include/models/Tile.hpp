#pragma once
#include <string>

#include "Player.hpp"
class Tile {
protected:
    int index;
    std::string name;
    std::string code;
    std::string color;
    
public:
    Tile(int index, std::string name, std::string code, std::string color);
    virtual ~Tile() = default;
    int getIndex() const;
    std::string getName() const;
    std::string getCode() const;
    std::string getColor() const;
    virtual void onLand(Player& p) = 0;
};

enum CardType {
    CHANCE,
    COMMUNITY_CHEST
};

class CardTile : public Tile {
private:
    CardType card;

public:
    CardTile(int index, std::string name, std::string code, std::string color, CardType card);
    CardType getType();
    void onLand(Player& p) override;
};

enum TaxType {
    PPH,
    PBM
};

class TaxTile : public Tile {
private:
    TaxType tax_type;
    float tax_amount;

public:
    TaxTile(int index, std::string name, std::string code, std::string color, TaxType tax_type, float tax_amount);
    float getTaxAmount();
    void onLand(Player& p) override;
};

class FestivalTile : public Tile {
public:
    FestivalTile(int index, std::string name, std::string code, std::string color);
    void onLand(Player& p) override;
};

class GoTile : public Tile {
private:
    float go_reward;

public:
    GoTile(int index, std::string name, std::string code, std::string color, float go_reward);
    float getReward();
    void onLand(Player& p) override;
};

class GoToJailTile : public Tile {
public:
    GoToJailTile(int index, std::string name, std::string code, std::string color);
    void onLand(Player& p) override;
};

class FreeParkingTile : public Tile {
public:
    FreeParkingTile(int index, std::string name, std::string code, std::string color);
    void onLand(Player& p) override;
};

class JailTile : public Tile {
private:
    int jail_fine;

public:
    JailTile(int index, std::string name, std::string code, std::string color, int jail_fine);
    void onLand(Player& p) override;
};