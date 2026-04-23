#pragma once

#include "models/Plot/ActionPlot/CardPlot.hpp"

class ChanceCardPlot : public CardPlot {
public:
    ChanceCardPlot(std::string name, std::string code, Color color);
    ~ChanceCardPlot() = default;

    std::string getType() const override;
    void startEvent(PlotContext& ctx) override;
};
