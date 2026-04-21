#pragma once

#include <exception>
#include <string>
#include <iostream>
using namespace std;

#include "models/Money.hpp"

class NimonopoliException : public exception {
public:
    explicit NimonopoliException(string message);

    const char* what() const noexcept override;

private:
    string message;
};

class InsufficientFundsException final : public NimonopoliException {
public:
    InsufficientFundsException(Money required, Money available);

    Money getRequired() const;
    Money getAvailable() const;

private:
    Money required;
    Money available;
};

class InvalidCommandException final : public NimonopoliException {
public:
    explicit InvalidCommandException(string message);
};

class InvalidPropertyException final : public NimonopoliException {
public:
    explicit InvalidPropertyException(string message);
};

class CardSlotFullException final : public NimonopoliException {
public:
    explicit CardSlotFullException(int maxSlots);

    int getMaxSlots() const;

private:
    int maxSlots;
};

class InvalidBidException final : public NimonopoliException {
public:
    InvalidBidException(int bidAmount, string reason);

    int getBidAmount() const;
    const string& getReason() const;

private:
    int bidAmount;
    string reason;
};

class SaveLoadException final : public NimonopoliException {
public:
    explicit SaveLoadException(string message);
};

class InvalidBoardConfigException final : public NimonopoliException {
public:
    explicit InvalidBoardConfigException(string message);
};
