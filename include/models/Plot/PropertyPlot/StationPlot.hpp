#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class StationPlot: public PropertyPlot{
private:
    static std::map<int, int> rentPriceTable;

public:
    StationPlot(std::string name, std::string code, Color color, int buyPrice, int mortgageValue,
            Player* owner = NULL, PropertyStatus propertyStatus = PropertyStatus::BANK,
            int festivalDuration = 0, int festivalMultiplier = 1);
    ~StationPlot() = default;

    std::map<int, int> getRentPriceTable() const override;
    static void setRentPriceTable(std::map<int, int> rentPriceTable);
    int getRentPrice(int level) const override;

    int calculateRentPrice() const override;
    PlotType getType() const override;
    void startEvent(PlotContext& ctx) override;
};