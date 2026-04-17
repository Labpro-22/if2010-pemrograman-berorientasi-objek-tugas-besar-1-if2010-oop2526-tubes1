#ifndef LOADER_HPP
#define LOADER_HPP
// #include "core/Player.hpp"
// #include "core/CardManager.hpp"
// #include "core/Account.hpp"
#include <fstream>
#include <vector>
#include <map>
using namespace std;

class Loader
{
private:
    fstream fileStream;

public:
    // vector<Player> loadPlayers(string path);
    // vector<PropertyTile> loadProperties(string path);
    // CardManager loadDecks(string path);
    vector<int> loadLogs(string path);
    vector<tuple<int, string, string, string, string, int, int, vector<int>>> loadPropertiesConfig(string path);
    map<int, int> loadRailroadsConfig(string path);
    map<int, int> loadUtilitiesConfig(string path);
    tuple<int, int, int> loadTaxConfig(string path);
    tuple<int, int> loadSpecialConfig(string path);
    tuple<int, int> loadMiscConfig(string path);
    // vector<Account> loadAccounts(string path);
};

#endif