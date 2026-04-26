#include "models/Money.hpp"

#include <string>
using namespace std;

Money::Money(int amount) : amount(amount) {}

int Money::getAmount() const {
    return amount;
}

void Money::setAmount(int value) {
    amount = value;
}

bool Money::isZero() const {
    return amount == 0;
}

bool Money::isPositive() const {
    return amount > 0;
}

Money Money::operator+(const Money& other) const {
    return Money(amount + other.amount);
}

Money Money::operator-(const Money& other) const {
    return Money(amount - other.amount);
}

Money& Money::operator+=(const Money& other) {
    amount += other.amount;
    return *this;
}

Money& Money::operator-=(const Money& other) {
    amount -= other.amount;
    return *this;
}

bool Money::operator<(const Money& other) const {
    return amount < other.amount;
}

bool Money::operator>(const Money& other) const {
    return amount > other.amount;
}

bool Money::operator<=(const Money& other) const {
    return amount <= other.amount;
}

bool Money::operator>=(const Money& other) const {
    return amount >= other.amount;
}

bool Money::operator==(const Money& other) const {
    return amount == other.amount;
}

bool Money::operator!=(const Money& other) const {
    return amount != other.amount;
}

Money Money::zero() {
    return Money(0);
}

string Money::toString() const {
    return "M" + to_string(amount);
}
