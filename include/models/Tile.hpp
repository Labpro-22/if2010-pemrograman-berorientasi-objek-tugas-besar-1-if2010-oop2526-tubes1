#pragma once
#include <string>

class Player;
class TileVisitor;

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
    virtual void onLand(Player& p, TileVisitor& visitor) = 0;
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
    void onLand(Player& p, TileVisitor& visitor) override;
};

enum TaxType {
    PPH,
    PBM
};

class TaxTile : public Tile {
private:
    TaxType tax_type;
    int tax_amount;

public:
    TaxTile(int index, std::string name, std::string code, std::string color, TaxType tax_type, int tax_amount);
    int getTaxAmount();
    void onLand(Player& p, TileVisitor& visitor) override;
};

class FestivalTile : public Tile {
public:
    FestivalTile(int index, std::string name, std::string code, std::string color);
    void onLand(Player& p, TileVisitor& visitor) override;
};

class GoTile : public Tile {
private:
    int go_reward;

public:
    GoTile(int index, std::string name, std::string code, std::string color, int go_reward);
    int getReward();
    void onLand(Player& p, TileVisitor& visitor) override;
};

class GoToJailTile : public Tile {
public:
    GoToJailTile(int index, std::string name, std::string code, std::string color);
    void onLand(Player& p, TileVisitor& visitor) override;
};

class FreeParkingTile : public Tile {
public:
    FreeParkingTile(int index, std::string name, std::string code, std::string color);
    void onLand(Player& p, TileVisitor& visitor) override;
};

class JailTile : public Tile {
private:
    int jail_fine;

public:
    JailTile(int index, std::string name, std::string code, std::string color, int jail_fine);
    void onLand(Player& p, TileVisitor& visitor) override;
};