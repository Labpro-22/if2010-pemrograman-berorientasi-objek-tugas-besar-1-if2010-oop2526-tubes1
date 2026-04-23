#include "models/Plot/SpecialPlot/GoPrisonPlot.hpp"

GoPrisonPlot::GoPrisonPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

std::string GoPrisonPlot::getType() const {
    return "Petak Pergi ke Penjara";
}

void GoPrisonPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}