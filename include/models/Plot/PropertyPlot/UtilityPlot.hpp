#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class UtilityPlot: public PropertyPlot{
private:
    static std::map<int, int> rentPriceTable;

public:
    UtilityPlot(std::string name, std::string code, Color color, int mortgageValue,
            PropertyStatus propertyStatus = PropertyStatus::BANK);
    ~UtilityPlot() = default;
    
    std::map<int, int> getRentPriceTable() const override;
    void setRentPriceTable(std::map<int, int> rentPriceTable);
    int getRentPrice(int level) const override;

    int calculateRentPrice() const override;
    std::string getType() const override;
    void startEvent(PlotContext& ctx) override;
};