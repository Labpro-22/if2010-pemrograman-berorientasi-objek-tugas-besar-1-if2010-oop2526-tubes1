#include "models/Plot/SpecialPlot/GoPrisonPlot.hpp"

GoPrisonPlot::GoPrisonPlot(std::string name, std::string code)
    : SpecialPlot(name, code) {}

std::string GoPrisonPlot::getType() const {
    return "Petak Pergi ke Penjara";
}

void GoPrisonPlot::startEvent() {
    //TODO (PlotContext)
}