#include "models/Plot/SpecialPlot/StartPlot.hpp"

StartPlot::StartPlot(std::string name, std::string code)
    : SpecialPlot(name, code) {}

std::string StartPlot::getType() const {
    return "Petak Mulai";
}

void StartPlot::startEvent() {
    //TODO (PlotContext)
}