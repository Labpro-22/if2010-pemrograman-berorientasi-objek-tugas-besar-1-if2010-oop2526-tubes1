#include "models/Plot/ActionPlot/FestivalPlot.hpp"

FestivalPlot::FestivalPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color) {}

PlotType FestivalPlot::getType() const {
    return PlotType::FESTIVALPLOT;
}

void FestivalPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}