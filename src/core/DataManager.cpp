#pragma once

#include "../../include/core/DataManager.hpp"
#include "../../include/core/GameManager.hpp"


DataManager::DataManager(const std::string& configMisc, const std::string& configProperty, const std::string& configTax, const std::string& configUtility, const std::string& configRailroad) : configMisc(configMisc), configProperty(configProperty), configRailroad(configRailroad), configTax(configTax), configUtility(configUtility) {}

void DataManager::loadMisc(GameManager& game) {
    ifstream file(configMisc);
    if (!file.is_open()) {       
        return;
    }

    string headerMaxturn, headerSaldoAwal;
    int tokenMaxturn, tokenSaldoAwal;

    file >> headerMaxturn >> headerSaldoAwal;
    file >> tokenMaxturn >> tokenSaldoAwal;  
    
    game.setMaxTurn(tokenMaxturn);
    game.setAllPlayersCurrency(tokenSaldoAwal);
    file.close();
}

void DataManager::loadProperties(GameManager& game, const vector<int>& utilityRent, const std::vector<int>& railroadRent) {
    ifstream file(configProperty);
    if (!file.is_open()) {       
        return;
    }

    string header;
    getline(file, header);

    int id, landCost, mortgageValue, houseCost, hotelCost;
    string code, name, type, color;

}

vector<int> DataManager::loadRailroadConfig() {
    ifstream file(configRailroad);
    if (!file.is_open()) {       
        return;
    }

    vector<int> mortgageRange;
    string header;
    getline(file, header);    

    int totalRailRoad, mortageVal;
    while (file >> totalRailRoad >> mortageVal) {
        mortgageRange.push_back(mortageVal);
    }
    file.close();
    return mortgageRange;
}

void DataManager::loadTax(GameManager& game) {
    ifstream file(configTax);
    if (!file.is_open()) {       
        return;
    }    
    string header;
    getline(file, header);

    
}