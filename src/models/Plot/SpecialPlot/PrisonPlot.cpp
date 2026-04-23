#include "models/Plot/SpecialPlot/PrisonPlot.hpp"#include "models/Plot/SpecialPlot/PrisonPlot.hpp"

PrisonPlot::PrisonPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

PlotType PrisonPlot::getType() const {
    return PlotType::PRISONPLOT;
}

void PrisonPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}