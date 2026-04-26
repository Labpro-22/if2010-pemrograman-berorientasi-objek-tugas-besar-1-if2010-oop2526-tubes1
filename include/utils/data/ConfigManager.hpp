#pragma once
#include <string>
#include "models/base/GameTypes.hpp"

class ConfigManager {
public:
    ConfigManager() = default;

    GameConfig loadGameConfig(const std::string& configDirectory) const;
    GameSaveData loadGameState(const std::string& filePath) const;
    void saveGameState(const std::string& filePath, const GameSaveData& saveData) const;
};
