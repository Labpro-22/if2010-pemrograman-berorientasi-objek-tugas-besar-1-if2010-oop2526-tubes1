#include "../../include/core/GameException.hpp"

GameException::GameException(const std::string& msg) : message(msg) {}

const char* GameException::what() const noexcept {
    return message.c_str();
}

InputException::InputException(const std::string& msg) 
    : GameException("[Input Error]: " + msg) {}

ConfigException::ConfigException(const std::string& msg) 
    : GameException("[Config Error]: " + msg) {}

SaveLoadException::SaveLoadException(const std::string& msg) 
    : GameException("[Save/Load Error]: " + msg) {}

GameplayException::GameplayException(const std::string& msg) 
    : GameException("[Gameplay Violation]: " + msg) {}