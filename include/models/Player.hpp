#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../../include/models/Saveable.hpp"
#include "../../include/models/Effect.hpp"
#include "../../include/models/SkillCard.hpp"

class PropertyTile;

enum PlayerState {
    FREE,
    INJAIL
};

class Player : public Saveable {
private:
    static std::random_device rd;
    static std::mt19937 gen;
    std::string name;
    int balance;
    int position;
    PlayerState player_state;
    std::vector<PropertyTile*> owned_properties;
    std::vector<std::unique_ptr<SkillCard>> saved_cards;
    std::vector<std::unique_ptr<Effect>> active_effects;

public:
    Player(std::string name, int balance, int position, PlayerState player_state);
    std::string getname();
    Player& operator+=(int amount);
    Player& operator-=(int amount);
    Player operator+(int amount) const;
    Player operator-(int amount) const;    
    void transferTo(Player& player, int amount); //DO NOT USE THIS FOR AUCTION OR LAND BUYING gitu, kalo mau itu pake sellProperty aja
    void movePlayer(int steps);
    void addProperty(PropertyTile* property);
    void removeProperty(PropertyTile* property);
    int countOwnedRailroad() const;
    int countOwnedUtility() const;

    void addSkillCard(std::unique_ptr<SkillCard> card);
    void useSkillCard(int index,Board& b, std::vector<Player>& all, TileVisitor& visitor);
    void addEffect(std::unique_ptr<Effect> effect);

    int getTotalAssetValue();
    bool canPay(int amount);
    void buyProperty(PropertyTile &property);
    void sellProperty(PropertyTile &property, Player& other);
    int liquidateAsset(int required);
    void declareBankruptcy();

    bool inJail();
    void setInJail();
    void setFree();
    void startTurn(Board &board);
    void endTurn();
    int getPosition();

    //Saveload impl
    std::string toSaveFormat() const;
};