#pragma once

#include "SkillCard.hpp"

class MoveCard : public SkillCard {
public:
    explicit MoveCard(int value = 0);
    void apply(Player& player, GameEngine& game) override;
    std::string getTypeName() const override;
};

class DiscountCard : public SkillCard {
public:
    explicit DiscountCard(int value = 0);
    void apply(Player& player, GameEngine& game) override;
    std::string getTypeName() const override;
};

class ShieldCard : public SkillCard {
public:
    ShieldCard();
    void apply(Player& player, GameEngine& game) override;
    std::string getTypeName() const override;
};

class TeleportCard : public SkillCard {
private:
    std::string targetCode;

public:
    TeleportCard();
    void setTargetCode(const std::string& code);
    void apply(Player& player, GameEngine& game) override;
    std::string getTypeName() const override;
};

class LassoCard : public SkillCard {
private:
    std::string targetUsername;

public:
    LassoCard();
    void setTargetUsername(const std::string& username);
    void apply(Player& player, GameEngine& game) override;
    std::string getTypeName() const override;
};

class DemolitionCard : public SkillCard {
private:
    std::string targetPropertyCode;

public:
    DemolitionCard();
    void setTargetPropertyCode(const std::string& code);
    void apply(Player& player, GameEngine& game) override;
    std::string getTypeName() const override;
};
