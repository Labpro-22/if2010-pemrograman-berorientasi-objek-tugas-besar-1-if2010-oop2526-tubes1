#pragma once
#include "GameException.hpp"

class InsufficientFundException : public GameException
{
private:
    int requiredAmount;
    int currentAmount;
public:
    InsufficientFundException(int requiredAmount, int currentAmount);
    int getDeficit() const;
};