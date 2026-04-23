#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include <memory>
#include "core/GameException.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include "models/Plot/PropertyPlot/StationPlot.hpp"
#include "models/Plot/PropertyPlot/UtilityPlot.hpp"
#include "models/Plot/ActionPlot/ActionPlot.hpp"
#include "models/Plot/ActionPlot/ChanceCardPlot.hpp"
#include "models/Plot/ActionPlot/CommunityChestCardPlot.hpp"
#include "models/Plot/ActionPlot/FestivalPlot.hpp"
#include "models/Plot/ActionPlot/IncomeTaxPlot.hpp"
#include "models/Plot/ActionPlot/LuxuryTaxPlot.hpp"
#include "models/Plot/SpecialPlot/FreeParkPlot.hpp"
#include "models/Plot/SpecialPlot/GoPrisonPlot.hpp"
#include "models/Plot/SpecialPlot/PrisonPlot.hpp"
#include "models/Plot/SpecialPlot/StartPlot.hpp"

class ConfigLoader{
private:
    static std::ifstream open(std::string path);

    static std::map<int, int> loadIntMap(std::string path);

    static std::tuple<int, int> loadTuple2(std::string path);
    
    static std::tuple<int, int, int> loadTuple3(std::string path);

    static Color colorTypeToEnum(std::string color);

public:
    static std::vector<std::pair<int, std::unique_ptr<Plot>>> loadProperty(std::string path);

    static std::map<int, int> loadRailroad(std::string path);

    static std::map<int, int> loadUtility(std::string path);

    static std::vector<std::pair<int, std::unique_ptr<Plot>>> loadAction(std::string path);

    static std::tuple<int, int, int> loadTax(std::string path);

    static std::tuple<int, int> loadSpecial(std::string path);

    static std::tuple<int, int> loadMisc(std::string path);
};