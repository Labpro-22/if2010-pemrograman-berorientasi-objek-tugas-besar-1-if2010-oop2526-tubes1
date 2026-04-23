#include "models/Plot/ActionPlot/FestivalPlot.hpp"

FestivalPlot::FestivalPlot(std::string name, std::string code)
    : ActionPlot(name, code) {}

std::string FestivalPlot::getType() const {
    return "Petak Festival";
}

void FestivalPlot::startEvent() {
    //TODO (PlotContext)
}