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
    INJAIL,
    BANKCRUPT
};

class Player : public Saveable, public std::enable_shared_from_this<Player> {
private:
    static std::random_device rd;
    static std::mt19937 gen;
    std::string name;
    float balance;
    int position;
    PlayerState player_state;
    std::vector<std::unique_ptr<SkillCard>> saved_cards;
    std::vector<std::unique_ptr<Effect>> active_effects;

public:
    std::vector<PropertyTile*> owned_properties;

    Player(std::string name, float balance, int position, PlayerState player_state);
    std::string getname();
    void receive(int amount);
    void pay(int amount);
    Player& operator+=(float amount);
    Player& operator-=(float amount);
    Player operator+(float amount) const;
    Player operator-(float amount) const;    
    void transferTo(Player& player, float amount); //DO NOT USE THIS FOR AUCTION OR LAND BUYING gitu, kalo mau itu pake sellProperty aja
    void movePlayer(int steps);
    void addProperty(PropertyTile* property);
    void removeProperty(PropertyTile* property);
    int countOwnedRailroad() const;
    int countOwnedUtility() const;
    float getmoney();
    void addSkillCard(std::unique_ptr<SkillCard> card);
    void useSkillCard(int index);
    void addEffect(std::unique_ptr<Effect> effect);

    float getTotalAssetValue();
    bool canPay(float amount);
    void buyProperty(PropertyTile &property);
    void sellProperty(PropertyTile &property);
    float liquidateAsset(float required);
    void declareBankruptcy();

    bool inJail();
    void setInJail();
    void setFree();
    void startTurn(int step);
    void endTurn();
    int getPosition();

    //Saveload impl
    std::string toSaveFormat() const;
};