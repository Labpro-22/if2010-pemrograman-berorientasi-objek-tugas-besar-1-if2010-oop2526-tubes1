#pragma once

#include "models/Plot/ActionPlot/ActionPlot.hpp"

class FestivalPlot : public ActionPlot {
public:
    FestivalPlot(std::string name, std::string code, Color color);
    ~FestivalPlot() = default;
    PlotType getType() const override;
    void startEvent(PlotContext& ctx) override;
};