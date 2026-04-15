#pragma once
#include <string>
#include <vector>
#include <memory>

class PropertyTile;

class SkillCard;

class Player {
private:
    std::string name;
    int balance;
    int position;
    std::vector<PropertyTile*> owned_properties;
    std::vector<std::unique_ptr<SkillCard>> saved_cards;
    std::vector<std::unique_ptr<Effect>> active_effects;

public:
    Player(std::string name, int balance, int position);
    Player& operator+=(int amount);
    Player& operator-=(int amount);
    Player operator+(int amount) const;
    Player operator-(int amount) const;
    int processPayment(int amount) const;
    int pay(int amount);
    void receive(int amount);       
    void transferTo(Player&, int); 
    void movePlayer(int steps);
    void addProperty(PropertyTile* property);
    void removeProperty(PropertyTile* property);
    void addSkillCard(std::unique_ptr<SkillCard> card);
    void useSkillCard(int index);
    void addEffect(std::unique_ptr<Effect> effect);
    int getTotalAssetValue();
    bool canPay(int amount);
    int liquidateAsset(int required);
    void declareBankruptcy();
    void setInJail();
    void setFree();
    void startTurn();
    void endTurn();
};

class Effect {
public:
    virtual ~Effect() = default;
    virtual void onTurnStart(Player&) {}
    virtual void onTurnEnd(Player&) {}
    virtual bool isExpired() const = 0;
    virtual bool blocksPayment() const;
    virtual int modifyPayment(int amount) const;
};

class ShieldEffect : public Effect {
private:
    int turns_left;

public:
    explicit ShieldEffect(int duration = 1);
    void onTurnEnd(Player&) override;
    bool isExpired() const override;
    bool blocksPayment() const override;
};

class DiscountEffect : public Effect {
private:
    int percent;
    int turns_left;

public:
    DiscountEffect(int percent, int duration = 1);
    void onTurnEnd(Player&) override;
    bool isExpired() const override;
    int modifyPayment(int amount) const override;
};