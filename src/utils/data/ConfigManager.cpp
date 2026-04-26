#include "utils/data/ConfigManager.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cctype>

namespace {
PropertyType parsePropertyType(const std::string& token) {
    if (token == "STREET" || token == "street") return PropertyType::STREET;
    if (token == "RAILROAD" || token == "railroad") return PropertyType::RAILROAD;
    return PropertyType::UTILITY;
}

ActionType parseActionType(const std::string& kindToken, const std::string& code) {
    if (kindToken == "FESTIVAL") return ActionType::FESTIVAL;
    if (kindToken == "PAJAK") {
        return code == "PBM" ? ActionType::TAX_PBM : ActionType::TAX_PPH;
    }
    if (code == "KSP") return ActionType::CHANCE;
    return ActionType::COMMUNITY_CHEST;
}

SpecialType parseSpecialType(const std::string& code) {
    if (code == "GO") return SpecialType::GO;
    if (code == "PEN") return SpecialType::JAIL;
    if (code == "PPJ") return SpecialType::GO_TO_JAIL;
    return SpecialType::FREE_PARKING;
}

PropertyStatus parsePropertyStatus(const std::string& token) {
    if (token == "OWNED") return PropertyStatus::OWNED;
    if (token == "MORTGAGED") return PropertyStatus::MORTGAGED;
    return PropertyStatus::BANK;
}

PlayerStatus parsePlayerStatus(const std::string& token) {
    if (token == "JAILED") return PlayerStatus::JAILED;
    if (token == "BANKRUPT") return PlayerStatus::BANKRUPT;
    return PlayerStatus::ACTIVE;
}

std::string propertyTypeToString(PropertyType type) {
    switch (type) {
        case PropertyType::STREET: return "street";
        case PropertyType::RAILROAD: return "railroad";
        case PropertyType::UTILITY: return "utility";
    }
    return "street";
}

std::string propertyStatusToString(PropertyStatus status) {
    switch (status) {
        case PropertyStatus::BANK: return "BANK";
        case PropertyStatus::OWNED: return "OWNED";
        case PropertyStatus::MORTGAGED: return "MORTGAGED";
    }
    return "BANK";
}

std::string playerStatusToString(PlayerStatus status) {
    switch (status) {
        case PlayerStatus::ACTIVE: return "ACTIVE";
        case PlayerStatus::JAILED: return "JAILED";
        case PlayerStatus::BANKRUPT: return "BANKRUPT";
    }
    return "ACTIVE";
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        parts.push_back(item);
    }
    return parts;
}

bool isIntegerToken(const std::string& token) {
    if (token.empty()) {
        return false;
    }
    std::size_t start = (token[0] == '-' || token[0] == '+') ? 1 : 0;
    if (start == token.size()) {
        return false;
    }
    for (std::size_t i = start; i < token.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(token[i]))) {
            return false;
        }
    }
    return true;
}
}

GameConfig ConfigManager::loadGameConfig(const std::string& configDirectory) const {
    GameConfig config;

    {
        std::ifstream file((configDirectory + "/aksi.txt").c_str());
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || !std::isdigit(static_cast<unsigned char>(line[0]))) continue;
            std::stringstream ss(line);
            ActionTileDefinition def;
            std::string kindToken;
            ss >> def.id >> def.code >> def.name >> kindToken >> def.colorGroup;
            def.isSpecial = (kindToken == "SPESIAL");
            if (def.isSpecial) {
                def.specialType = parseSpecialType(def.code);
            } else {
                def.actionType = parseActionType(kindToken, def.code);
            }
            config.actionTiles.push_back(def);
        }
    }

    {
        std::ifstream file((configDirectory + "/property.txt").c_str());
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || !std::isdigit(line[0])) continue;
            std::stringstream ss(line);
            PropertyDefinition def;
            std::string typeToken;
            ss >> def.id >> def.code >> def.name >> typeToken >> def.colorGroup >> def.purchasePrice >> def.mortgageValue;
            def.type = parsePropertyType(typeToken);
            if (def.type == PropertyType::STREET) {
                ss >> def.houseCost >> def.hotelCost;
                int rentValue;
                while (ss >> rentValue) {
                    def.rentTable.push_back(rentValue);
                }
            }
            config.properties.push_back(def);
        }
        for (std::size_t i = 0; i < config.properties.size(); ++i) {
            if (config.properties[i].type != PropertyType::STREET) continue;
            int count = 0;
            for (std::size_t j = 0; j < config.properties.size(); ++j) {
                if (config.properties[j].type == PropertyType::STREET &&
                    config.properties[j].colorGroup == config.properties[i].colorGroup) {
                    ++count;
                }
            }
            config.properties[i].colorGroupSize = count;
        }
    }

    {
        std::ifstream file((configDirectory + "/railroad.txt").c_str());
        int k, v;
        while (file >> k >> v) {
            config.railroadRentByCount[k] = v;
        }
    }

    {
        std::ifstream file((configDirectory + "/utility.txt").c_str());
        int k, v;
        while (file >> k >> v) {
            config.utilityMultiplierByCount[k] = v;
        }
    }

    {
        std::ifstream file((configDirectory + "/tax.txt").c_str());
        int a, b, c;
        while (file >> a >> b >> c) {
            config.tax.pphFlat = a;
            config.tax.pphPercentage = b;
            config.tax.pbmFlat = c;
        }
    }

    {
        std::ifstream file((configDirectory + "/special.txt").c_str());
        int a, b;
        while (file >> a >> b) {
            config.special.goSalary = a;
            config.special.jailFine = b;
        }
    }

    {
        std::ifstream file((configDirectory + "/misc.txt").c_str());
        int a, b;
        while (file >> a >> b) {
            config.misc.maxTurn = a;
            config.misc.startingBalance = b;
        }
    }

    return config;
}

GameSaveData ConfigManager::loadGameState(const std::string& filePath) const {
    GameSaveData save;
    std::ifstream file(filePath.c_str());
    if (!file) {
        throw std::runtime_error("Cannot open save file.");
    }

    int playerCount = 0;
    file >> save.currentTurn >> save.maxTurn >> playerCount;
    std::string dummy;
    std::getline(file, dummy);

    for (int i = 0; i < playerCount; ++i) {
        std::string line;
        std::getline(file, line);
        if (line.empty()) { --i; continue; }
        SavedPlayerState state;
        std::vector<std::string> tokens = split(line, ' ');
        if (tokens.size() < 6) {
            throw std::runtime_error("Invalid player entry in save file.");
        }

        std::size_t index = 0;
        state.name = tokens[index++];
        state.money = std::atoi(tokens[index++].c_str());
        if (index < tokens.size() && isIntegerToken(tokens[index])) {
            state.position = std::atoi(tokens[index++].c_str());
        }
        if (index + 3 > tokens.size()) {
            throw std::runtime_error("Incomplete player entry in save file.");
        }

        state.tileCode = tokens[index++];
        std::string status = tokens[index++];
        state.status = parsePlayerStatus(status);
        state.jailTurnsSpent = std::atoi(tokens[index++].c_str());
        int cardCount = std::atoi(tokens[index++].c_str());
        if (tokens.size() < index + static_cast<std::size_t>(cardCount * 3)) {
            throw std::runtime_error("Incomplete skill card data in save file.");
        }
        for (int j = 0; j < cardCount; ++j) {
            SavedSkillCardState card;
            card.cardName = tokens[index++];
            card.value = std::atoi(tokens[index++].c_str());
            card.remainingDuration = std::atoi(tokens[index++].c_str());
            state.skillCards.push_back(card);
        }
        save.players.push_back(state);
    }

    std::string tag;
    file >> tag;
    if (tag == "TURN_ORDER") {
        int count;
        file >> count;
        for (int i = 0; i < count; ++i) {
            std::string name;
            file >> name;
            save.turnOrderNames.push_back(name);
        }
    }
    file >> tag >> save.currentPlayerIndex;
    file >> tag;
    if (tag == "PROPERTY_COUNT") {
        int count;
        file >> count;
        std::getline(file, dummy);
        for (int i = 0; i < count; ++i) {
            std::string line;
            std::getline(file, line);
            if (line.empty()) { --i; continue; }
            std::stringstream ss(line);
            SavedPropertyState state;
            std::string typeStr;
            std::string statusStr;
            ss >> state.code >> typeStr >> state.ownerName >> statusStr >> state.festivalMultiplier >> state.festivalTurnsRemaining >> state.buildingLevel;
            state.propertyType = parsePropertyType(typeStr);
            state.status = parsePropertyStatus(statusStr);
            save.properties.push_back(state);
        }
    }

    file >> tag;
    if (tag == "SKILL_DECK") {
        int count;
        file >> count;
        for (int i = 0; i < count; ++i) {
            std::string name;
            file >> name;
            save.skillDeckState.skillDeckCardNames.push_back(name);
        }
    }
    std::getline(file, dummy);

    std::string line;
    std::getline(file, line);
    if (!line.empty()) {
        std::stringstream ss(line);
        ss >> tag;
        if (tag == "LOG_COUNT") {
            int count;
            ss >> count;
            for (int i = 0; i < count; ++i) {
                std::getline(file, line);
                std::vector<std::string> parts = split(line, '|');
                if (parts.size() >= 4) {
                    save.logEntries.push_back(LogEntry(std::atoi(parts[0].c_str()), parts[1], parts[2], parts[3]));
                }
            }
        }
    }

    return save;
}

void ConfigManager::saveGameState(const std::string& filePath, const GameSaveData& saveData) const {
    std::ofstream file(filePath.c_str());
    if (!file) {
        throw std::runtime_error("Cannot write save file.");
    }

    file << saveData.currentTurn << ' ' << saveData.maxTurn << ' ' << saveData.players.size() << '\n';
    for (std::size_t i = 0; i < saveData.players.size(); ++i) {
        const SavedPlayerState& p = saveData.players[i];
        file << p.name << ' ' << p.money << ' ' << p.position << ' ' << p.tileCode << ' ' << playerStatusToString(p.status)
             << ' ' << p.jailTurnsSpent << ' ' << p.skillCards.size();
        for (std::size_t j = 0; j < p.skillCards.size(); ++j) {
            file << ' ' << p.skillCards[j].cardName << ' ' << p.skillCards[j].value << ' ' << p.skillCards[j].remainingDuration;
        }
        file << '\n';
    }

    file << "TURN_ORDER " << saveData.turnOrderNames.size();
    for (std::size_t i = 0; i < saveData.turnOrderNames.size(); ++i) {
        file << ' ' << saveData.turnOrderNames[i];
    }
    file << '\n';

    file << "CURRENT_PLAYER " << saveData.currentPlayerIndex << '\n';
    file << "PROPERTY_COUNT " << saveData.properties.size() << '\n';
    for (std::size_t i = 0; i < saveData.properties.size(); ++i) {
        const SavedPropertyState& p = saveData.properties[i];
        file << p.code << ' ' << propertyTypeToString(p.propertyType) << ' ' << p.ownerName << ' '
             << propertyStatusToString(p.status) << ' ' << p.festivalMultiplier << ' '
             << p.festivalTurnsRemaining << ' ' << p.buildingLevel << '\n';
    }

    file << "SKILL_DECK " << saveData.skillDeckState.skillDeckCardNames.size();
    for (std::size_t i = 0; i < saveData.skillDeckState.skillDeckCardNames.size(); ++i) {
        file << ' ' << saveData.skillDeckState.skillDeckCardNames[i];
    }
    file << '\n';

    file << "LOG_COUNT " << saveData.logEntries.size() << '\n';
    for (std::size_t i = 0; i < saveData.logEntries.size(); ++i) {
        file << saveData.logEntries[i].turnNumber << '|'
             << saveData.logEntries[i].username << '|'
             << saveData.logEntries[i].actionName << '|'
             << saveData.logEntries[i].detail << '\n';
    }
}
