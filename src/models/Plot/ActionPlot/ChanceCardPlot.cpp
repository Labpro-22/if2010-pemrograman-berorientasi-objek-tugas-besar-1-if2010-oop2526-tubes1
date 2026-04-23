#include "models/Plot/ActionPlot/ChanceCardPlot.hpp"

ChanceCardPlot::ChanceCardPlot(std::string name, std::string code, Color color)
    : CardPlot(name, code, color){}

std::string ChanceCardPlot::getType() const {
    return "Petak Kesempatan";
}

void ChanceCardPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}