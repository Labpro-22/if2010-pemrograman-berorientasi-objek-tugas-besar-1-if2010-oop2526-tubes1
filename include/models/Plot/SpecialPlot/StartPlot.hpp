#pragma once

#include "models/Plot/SpecialPlot/SpecialPlot.hpp"

class StartPlot : public SpecialPlot {
public:
    StartPlot(std::string name, std::string code, Color color);
    ~StartPlot() = default;

    std::string getType() const override;
    void startEvent(PlotContext& ctx) override;
};