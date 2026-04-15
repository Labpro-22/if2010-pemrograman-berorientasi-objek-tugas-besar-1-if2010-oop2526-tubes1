#pragma once

#include "core/GameException/GameException.hpp"

class BuildingIsFullException : public GameException{
public:
    BuildingIsFullException(): GameException(GameErrorID::BUILDINGISFULLEXCEPTION,
        "Gagal membangun, bangunan sudah mencapai level maksimum.") {}
};