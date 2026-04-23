#include "models/Plot/ActionPlot/ChanceCardPlot.hpp"

ChanceCardPlot::ChanceCardPlot(std::string name, std::string code)
    : CardPlot(name, code){}

std::string ChanceCardPlot::getType() const {
    return "Petak Kesempatan";
}

void ChanceCardPlot::startEvent() {
    //TODO (PlotContext)
}