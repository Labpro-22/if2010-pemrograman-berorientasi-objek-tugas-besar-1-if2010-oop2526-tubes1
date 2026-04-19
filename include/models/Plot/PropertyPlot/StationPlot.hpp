#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class StationPlot: public PropertyPlot{
private:
    static std::map<int, int> rentPriceTable;

public:
    StationPlot(std::string name, std::string code, Color color, int mortgageValue,
            PropertyStatus propertyStatus = PropertyStatus::BANK);
    ~StationPlot() = default;

    std::map<int, int> getRentPriceTable() const override;
    void setRentPriceTable(std::map<int, int> rentPriceTable);
    int getRentPrice(int level) const override;

    int calculateRentPrice() const override;
    std::string getType() const override;
    void startEvent() override;
};