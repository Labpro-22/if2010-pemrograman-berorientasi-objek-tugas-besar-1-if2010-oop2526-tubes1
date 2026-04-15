#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class UtilityPlot: public PropertyPlot{
private:
    std::map<int, int> rentPrice;

public:
    UtilityPlot();
    ~UtilityPlot() = default;
    
    std::map<int, int> getRentPrice() const;
    void setRentPrice(std::map<int, int> rentPriceTable);

    int calculateRentPrice() const override;
    std::string getType() const override;
    Color getColor() const override;
    void startEvent() override;
};