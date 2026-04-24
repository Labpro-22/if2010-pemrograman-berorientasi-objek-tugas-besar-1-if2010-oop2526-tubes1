#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../../include/models/PropertyTile.hpp"
#include "../../include/models/StreetTile.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/models/Board.hpp" 
#include "../../include/utils/SaveData.hpp" 
#include "../../include/utils/ConfigData.hpp"

class IOManager {
public:
    IOManager() = default;
    ~IOManager() = default;

    GameSaveData loadGameData(const std::string& filepath);
    void saveGameData(const GameSaveData& data, const std::string& filepath);
    static FullConfigData loadAllConfigs(const std::string& configDirectoryPath);
    
    private:
    std::vector<std::unique_ptr<Tile>> IOparsePropertyTileConfig(const std::string& configDirectoryPath);
    std::vector<std::unique_ptr<Tile>> IOparseActionTileConfig(const std::string& configDirectoryPath, std::vector<int>& tax, std::vector<int>& special);
    std::vector<int> IOparseTaxConfig(const std::string& configDirectoryPath);
    std::vector<int> IOparseSpecialConfig(const std::string& configDirectoryPath);
    std::vector<int> IOparseMiscConfig(const std::string& configDirectoryPath);
    std::map<int,int> IOparseRailroadConfig(const std::string& configDirectoryPath);
    std::map<int,int> IOparseUtilityConfig(const std::string& configDirectoryPath);
};