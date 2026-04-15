#pragma once

#include <exception>
#include <string>
#include "core/GameException/GameErrorID.hpp"

class GameException : public std::exception{
private:
    GameErrorID errorID;
    std::string errorMsg;

public:
    GameException(GameErrorID errorID, std::string errorMsg):
        errorID(errorID), errorMsg(errorMsg) {}

    int getID() const {
        return static_cast<int>(errorID);
    }

    const char* what() const noexcept override {
        return errorMsg.c_str();
    }
};