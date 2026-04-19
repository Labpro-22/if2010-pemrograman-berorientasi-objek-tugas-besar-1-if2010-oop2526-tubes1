#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include "core/GameException.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include "models/Plot/PropertyPlot/StationPlot.hpp"
#include "models/Plot/PropertyPlot/UtilityPlot.hpp"

class ConfigLoader{
private:
    static std::ifstream open(std::string path);

    static std::map<int, int> loadIntMap(std::string path);

    static std::tuple<int, int> loadTuple2(std::string path);
    
    static std::tuple<int, int, int> loadTuple3(std::string path);

    static Color colorTypeToEnum(std::string color);

public:
    //TODO: pake unique_ptr biar lebih aman
    static std::vector<std::pair<int, Plot*>> loadProperty(std::string path);

    static std::map<int, int> loadRailroad(std::string path);

    static std::map<int, int> loadUtility(std::string path);

    static std::tuple<int, int, int> loadTax(std::string path);

    static std::tuple<int, int> loadSpecial(std::string path);

    static std::tuple<int, int> loadMisc(std::string path);
};