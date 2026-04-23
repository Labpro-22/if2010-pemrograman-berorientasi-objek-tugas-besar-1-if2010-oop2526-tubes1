#pragma once
 
#include "models/Plot/ActionPlot/CardPlot.hpp"
 
class CommunityChestCardPlot : public CardPlot {
public:
    CommunityChestCardPlot(std::string name, std::string code, Color color);
    ~CommunityChestCardPlot() = default;
 
    void startEvent(PlotContext& ctx) override;
};
 
