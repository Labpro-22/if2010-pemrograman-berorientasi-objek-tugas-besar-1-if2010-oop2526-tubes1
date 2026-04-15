#pragma once

#include "core/GameException/GameException.hpp"

class BuildingIsEmptyException : public GameException{
public:
    BuildingIsEmptyException(): GameException(GameErrorID::BUILDINGISEMPTYEXCEPTION,
        "Tidak ada bangunan yang bisa dijual pada petak ini.") {}
};