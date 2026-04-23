#include "models/Plot/SpecialPlot/StartPlot.hpp"

StartPlot::StartPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

std::string StartPlot::getType() const {
    return "Petak Mulai";
}

void StartPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}