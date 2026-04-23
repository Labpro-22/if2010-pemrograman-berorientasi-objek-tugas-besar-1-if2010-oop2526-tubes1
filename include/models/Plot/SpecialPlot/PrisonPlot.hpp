#pragma once

#include "models/Plot/SpecialPlot/SpecialPlot.hpp"

class PrisonPlot : public SpecialPlot {
public:
    PrisonPlot(std::string name, std::string code, Color color);
    ~PrisonPlot() = default;

    std::string getType() const override;
    void startEvent(PlotContext& ctx) override;
};