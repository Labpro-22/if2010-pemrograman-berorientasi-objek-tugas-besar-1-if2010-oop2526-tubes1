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
    ConfigLoader();
public:
    static ConfigLoader* getInstance();
    ~ConfigLoader();
    void loadAllConfigs(); 
    void parsePropertyConfig(); 
    void parseActionTileConfig(); 
    void validateBoardLayout(); 
};