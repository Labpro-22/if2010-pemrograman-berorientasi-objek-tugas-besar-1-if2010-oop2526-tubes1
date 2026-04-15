#pragma once

#include "core/GameException/GameException.hpp"

class InvalidInputException : public GameException{
public:
    InvalidInputException(): GameException(GameErrorID::INVALIDINPUTEXCEPTION,
        "Input tidak valid.") {}
};