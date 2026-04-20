#ifndef NIMONSPOLI_EXCEPTION_HPP
#define NIMONSPOLI_EXCEPTION_HPP

#include <exception>
#include <string>

class NimonspoliException : public std::exception {
protected:
    std::string message;

public:
    NimonspoliException(std::string message);
    virtual const char* what() const noexcept override;
};


class InsufficientFundsException : public NimonspoliException {
public:
    InsufficientFundsException(std::string message);
};

class InvalidActionException : public NimonspoliException {
public:
    InvalidActionException(std::string message);
};

class CardSlotFullException : public NimonspoliException {
public:
    CardSlotFullException(std::string message);
};

class PropertyNotOwnedException : public NimonspoliException {
public:
    PropertyNotOwnedException(std::string message);
};

class FileNotFoundException : public NimonspoliException {
public:
    FileNotFoundException(std::string message);
};

class InvalidInputException : public NimonspoliException {
public:
    InvalidInputException(std::string message);
};

#endif