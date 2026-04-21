#include "utils/Exceptions.hpp"

#include <utility>
using namespace std;

NimonopoliException::NimonopoliException(string message) : message(std::move(message)) {}

const char* NimonopoliException::what() const noexcept {
    return message.c_str();
}

InsufficientFundsException::InsufficientFundsException(Money required, Money available)
    : NimonopoliException(
          "Insufficient funds: required " + required.toString() + ", available " + available.toString()),
      required(required),
      available(available) {}

Money InsufficientFundsException::getRequired() const {
    return required;
}

Money InsufficientFundsException::getAvailable() const {
    return available;
}

InvalidCommandException::InvalidCommandException(string message)
    : NimonopoliException("Invalid command: " + std::move(message)) {}

InvalidPropertyException::InvalidPropertyException(string message)
    : NimonopoliException("Invalid property: " + std::move(message)) {}

CardSlotFullException::CardSlotFullException(int maxSlots)
    : NimonopoliException("Card slots full (max " + to_string(maxSlots) + ")"), maxSlots(maxSlots) {}

int CardSlotFullException::getMaxSlots() const {
    return maxSlots;
}

InvalidBidException::InvalidBidException(int bidAmount, string reason)
    : NimonopoliException("Invalid bid " + to_string(bidAmount) + ": " + reason),
      bidAmount(bidAmount),
      reason(std::move(reason)) {}

int InvalidBidException::getBidAmount() const {
    return bidAmount;
}

const string& InvalidBidException::getReason() const {
    return reason;
}

SaveLoadException::SaveLoadException(string message) : NimonopoliException("Save/Load: " + std::move(message)) {}

InvalidBoardConfigException::InvalidBoardConfigException(string message)
    : NimonopoliException("Invalid board config: " + std::move(message)) {}
