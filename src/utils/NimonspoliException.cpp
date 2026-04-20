#include "../../include/utils/NimonspoliException.hpp"

NimonspoliException::NimonspoliException(std::string message) 
    : message(message) {}

const char* NimonspoliException::what() const noexcept {
    return message.c_str();
}

InsufficientFundsException::InsufficientFundsException(std::string message) 
    : NimonspoliException(message) {}

InvalidActionException::InvalidActionException(std::string message) 
    : NimonspoliException(message) {}

CardSlotFullException::CardSlotFullException(std::string message) 
    : NimonspoliException(message) {}

PropertyNotOwnedException::PropertyNotOwnedException(std::string message) 
    : NimonspoliException(message) {}

FileNotFoundException::FileNotFoundException(std::string message) 
    : NimonspoliException(message) {}

InvalidInputException::InvalidInputException(std::string message) 
    : NimonspoliException(message) {}