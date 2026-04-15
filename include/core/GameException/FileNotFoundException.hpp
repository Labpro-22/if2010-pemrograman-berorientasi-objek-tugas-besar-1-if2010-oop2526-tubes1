#pragma once

#include "core/GameException/GameException.hpp"

class FileNotFoundException : public GameException{
public:
    FileNotFoundException(std::string path): GameException(GameErrorID::FILENOTFOUNDEXCEPTION,
        "Tidak ditemukan file pada path: " + path) {}
};