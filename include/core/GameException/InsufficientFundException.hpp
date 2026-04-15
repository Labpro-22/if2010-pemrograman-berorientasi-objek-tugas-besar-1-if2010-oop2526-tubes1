#pragma once

#include "core/GameException/GameException.hpp"

class InsufficientFundException : public GameException{
public:
    InsufficientFundException(): GameException(GameErrorID::INSUFFICIENTFUNDEXCEPTION,
        "Uang tidak cukup untuk melakukan aksi ini.") {}
};