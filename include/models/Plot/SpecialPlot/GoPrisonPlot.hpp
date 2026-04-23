#pragma once

#include "models/Plot/SpecialPlot/SpecialPlot.hpp"

class GoPrisonPlot : public SpecialPlot {
public:
    GoPrisonPlot(std::string name, std::string code, Color color);
    ~GoPrisonPlot() = default;

    void startEvent(PlotContext& ctx) override;
};