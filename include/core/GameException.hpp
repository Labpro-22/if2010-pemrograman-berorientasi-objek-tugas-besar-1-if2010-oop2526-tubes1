#pragma once

#include <exception>
#include <string>

enum class GameErrorID{
    INVALIDINPUTEXCEPTION,
    FILENOTFOUNDEXCEPTION,
    INVALIDFILEDATAEXCEPTION,
    UNKNOWNTYPEEXCEPTION,
    UNKNOWNCOLOREXCEPTION,
    INSUFFICIENTFUNDEXCEPTION,
    BUILDINGISFULLEXCEPTION,
    BUILDINGISEMPTYEXCEPTION,
    COLORSETNOTOWNEDEXCEPTION,
    NOACCESSTOPROPERTYEXCEPTION
};

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


class InvalidInputException : public GameException{
public:
    InvalidInputException(): GameException(GameErrorID::INVALIDINPUTEXCEPTION,
        "Input tidak valid.") {}
};

class FileNotFoundException : public GameException{
public:
    FileNotFoundException(std::string path): GameException(GameErrorID::FILENOTFOUNDEXCEPTION,
        "Tidak ditemukan file pada path: " + path) {}
};

class InvalidFileDataException : public GameException{
public:
    InvalidFileDataException(): GameException(GameErrorID::INVALIDFILEDATAEXCEPTION,
        "Terdapat data yang tidak valid dalam file.") {}
};

class UnknownTypeException : public GameException{
public:
    UnknownTypeException(std::string type): GameException(GameErrorID::UNKNOWNTYPEEXCEPTION,
        "Jenis property " + type + " tidak dikenali oleh program.") {}
};

class UnknownColorException : public GameException{
public:
    UnknownColorException(std::string color): GameException(GameErrorID::UNKNOWNCOLOREXCEPTION,
        "Warna " + color + " tidak dikenali oleh program.") {}
};

class InsufficientFundException : public GameException{
public:
    InsufficientFundException(): GameException(GameErrorID::INSUFFICIENTFUNDEXCEPTION,
        "Uang tidak cukup untuk melakukan aksi ini.") {}
};

class BuildingIsFullException : public GameException{
public:
    BuildingIsFullException(): GameException(GameErrorID::BUILDINGISFULLEXCEPTION,
        "Gagal membangun, bangunan sudah mencapai level maksimum.") {}
};

class BuildingIsEmptyException : public GameException{
public:
    BuildingIsEmptyException(): GameException(GameErrorID::BUILDINGISEMPTYEXCEPTION,
        "Tidak ada bangunan yang bisa dijual pada petak ini.") {}
};

class ColorSetNotOwnedException : public GameException{
public:
    ColorSetNotOwnedException(): GameException(GameErrorID::COLORSETNOTOWNEDEXCEPTION,
        "Dapatkan semua petak dengan warna sama agar bisa membangun.") {}
};

class NoAccessToPropertyException : public GameException{
public:
    NoAccessToPropertyException(): GameException(GameErrorID::NOACCESSTOPROPERTYEXCEPTION,
        "Anda tidak memiliki akses untuk properti ini.") {}
};