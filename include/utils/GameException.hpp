#pragma once
#include <stdexcept>
#include <string>
using namespace std;

class GameException : public std::runtime_error {
public:
    explicit GameException(const string& message) : std::runtime_error(message) {}
};

// exception for conf

// file config not found or ga bisa dibuka
class ConfigFileNotFoundException : public GameException {
public:
    explicit ConfigFileNotFoundException(const string& filename)
        : GameException("Config file not found: " + filename) {}        
};

// format barid di file config ga sesuai
class ConfigParseException : public GameException {
public: 
    explicit ConfigParseException(const string& message)
        : GameException("Config parse error: " + message) {}
};

// exception for player

// invalid ops buat player bangkrut
class PlayerBankruptException : public GameException {
public: 
    explicit PlayerBankruptException(const string& username)
        : GameException("Player " + username + " is bankrupt.") {}
};

// not enough saldo buat transaksi
class InsufficientFundsException : public GameException {
public:
    explicit InsufficientFundsException(const string& username, int required, int available)
        : GameException("Player " + username + " has insufficient funds. Required: " + to_string(required) + ", has  " + to_string(available)) {}
};

// kartu full
class HandFullException : public GameException {
public:
    explicit HandFullException(const string& username)
        : GameException("Player " + username + "already has max skill cards.") {}
};

// index kartu invalid
class InvalidCardIndexException : public GameException {
public:
    explicit InvalidCardIndexException(int index)
        : GameException("Invalid card index: " + to_string(index)) {}
};




