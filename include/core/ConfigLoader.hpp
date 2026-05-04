#pragma once
#include <string>
#include <fstream>
using namespace std;

class ConfigLoader {
private:
    static ConfigLoader* instance; 
    ifstream currentFile;
    string currentLine;
    int currentParsedIndex;
    std::string configFilePath;
    bool isConfigValid;
    ConfigLoader();
public:
    static ConfigLoader* getInstance();
    ~ConfigLoader();
    void loadAllConfigs(); 
    void parsePropertyConfig(); 
    void parseActionTileConfig(); 
    void validateBoardLayout();
    void setConfigFilePath(const std::string& path);
    bool getIsConfigValid() const;
};