#pragma once

#include "models/Plot/Plot.hpp"

class SpecialPlot : public Plot {
public:
    SpecialPlot(std::string name, std::string code, Color color);
    virtual ~SpecialPlot() = default;

    PlotType getType() const override;
    virtual void startEvent(PlotContext& ctx) override = 0;
};