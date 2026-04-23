#pragma once

#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class LandPlot: public PropertyPlot{
private:
    int upgHousePrice;
    int upgHotelPrice;
    std::map<int, int> rentPriceTable;
    int level;

public:
    LandPlot(std::string name, std::string code, Color color,
            int buyPrice, int mortgageValue, int upgHousePrice, int upgHotelPrice,
            std::map<int, int> rentPriceTable, Player* owner = NULL,
            PropertyStatus propertyStatus = PropertyStatus::BANK,
            int festivalDuration = 0, int festivalMultiplier = 1);

    ~LandPlot() = default;

    int getUpgHousePrice() const;
    int getUpgHotelPrice() const;
    std::map<int, int> getRentPriceTable() const override;
    int getRentPrice(int level) const override;
    int getLevel() const;

    void build();
    void sellBuildings();
    bool canBuild() const;
    int getBuildCost() const;
    bool isStreetOwned() const;
    int calculateRentPrice() const override;
    std::string getType() const override;
    void startEvent(PlotContext& ctx) override;
};