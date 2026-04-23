#include "models/Plot/SpecialPlot/PrisonPlot.hpp"#include "models/Plot/SpecialPlot/PrisonPlot.hpp"

PrisonPlot::PrisonPlot(std::string name, std::string code)
    : SpecialPlot(name, code) {}

std::string PrisonPlot::getType() const {
    return "Petak Penjara";
}

void PrisonPlot::startEvent() {
    //TODO (PlotContext)
}