#include "models/Plot/ActionPlot/TaxPlot.hpp"

TaxPlot::TaxPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color), flatAmount(flatAmount) {}

PlotType TaxPlot::getType() const {
    return PlotType::TAXPLOT;
}

int TaxPlot::getFlatAmount(PlotContext& ctx) const {
    return flatAmount;
}
