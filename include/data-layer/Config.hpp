#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <vector>
#include <map>
#include <string>
#include <tuple>
using namespace std;

class Config
{
private:
    vector<tuple<int, string, string, string, string, int, int, vector<int>>> propertyConfigs;
    map<int, int> railroadRentTable;
    map<int, int> utilityMultiplier;
    tuple<int, int, int> taxConfig;
    tuple<int, int> specialConfig;
    tuple<int, int> miscConfig;

public:
    tuple<int, string, string, string, string, int, int, vector<int>> getPropertyConfig(string code);
    int getRailroadRent(int count);
    int getUtilityMultiplier(int count);
};

#endif