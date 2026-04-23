#include "models/Plot/SpecialPlot/PrisonPlot.hpp"#include "models/Plot/SpecialPlot/PrisonPlot.hpp"

PrisonPlot::PrisonPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

std::string PrisonPlot::getType() const {
    return "Petak Penjara";
}

void PrisonPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}