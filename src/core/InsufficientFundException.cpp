#include "../../include/core/InsufficientFundException.hpp"

InsufficientFundException::InsufficientFundException(int requiredAmount, int currentAmount) 
    : GameException("uang tidak cukup"), requiredAmount(requiredAmount), currentAmount(currentAmount) {}
int InsufficientFundException::getDeficit() const {
    return requiredAmount - currentAmount;
}