#include "models/Plot/ActionPlot/TaxPlot.hpp"

TaxPlot::TaxPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color), flatAmount(flatAmount) {}

std::string TaxPlot::getType() const {
    return "Petak Pajak";
}

int TaxPlot::getFlatAmount(PlotContext& ctx) const {
    return flatAmount;
}
