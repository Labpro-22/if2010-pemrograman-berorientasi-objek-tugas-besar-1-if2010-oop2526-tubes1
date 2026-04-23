#include "models/Plot/ActionPlot/FestivalPlot.hpp"

FestivalPlot::FestivalPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color) {}

std::string FestivalPlot::getType() const {
    return "Petak Festival";
}

void FestivalPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}