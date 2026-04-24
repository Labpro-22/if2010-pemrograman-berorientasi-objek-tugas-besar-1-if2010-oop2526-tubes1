#include "models/Plot/SpecialPlot/StartPlot.hpp"

StartPlot::StartPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

PlotType StartPlot::getType() const {
    return PlotType::STARTPLOT;
}

void StartPlot::startEvent(PlotContext& ctx) {
    //TODO: tampilkan pesan saja, mekanisme salary dihandle di bagian turn manager
}