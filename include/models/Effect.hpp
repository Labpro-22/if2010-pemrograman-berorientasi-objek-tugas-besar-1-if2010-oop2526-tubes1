#pragma once

class Player;

class Effect {
public:
    virtual ~Effect() = default;
    virtual void onTurnStart(Player& p);
    virtual void onTurnEnd(Player& p);
    virtual bool isExpired() const = 0;
    virtual bool blocksPayment() const;
    virtual float modifyPayment(float amount) const;
};

class ShieldEffect : public Effect {
private:
    int turns_left;

public:
    ShieldEffect(int duration = 1);
    void onTurnEnd(Player& p) override;
    bool isExpired() const override;
    bool blocksPayment() const override;
};

class DiscountEffect : public Effect {
private:
    float percent;
    int turns_left;

public:
    DiscountEffect(int percent, int duration = 1);
    void onTurnEnd(Player& p) override;
    bool isExpired() const override;
    float modifyPayment(float amount) const override;
};
