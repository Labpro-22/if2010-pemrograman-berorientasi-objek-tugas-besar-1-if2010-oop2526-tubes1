#pragma once

#include "core/GameException/GameException.hpp"

class ColorSetNotOwnedException : public GameException{
public:
    ColorSetNotOwnedException(): GameException(GameErrorID::COLORSETNOTOWNEDEXCEPTION,
        "Dapatkan semua petak dengan warna sama agar bisa membangun.") {}
};