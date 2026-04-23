#include "models/Plot/ActionPlot/IncomeTaxPlot.hpp"

IncomeTaxPlot::IncomeTaxPlot(std::string name, std::string code, Color color)
    : TaxPlot(name, code, color) {}

int IncomeTaxPlot::getPercentage() const {
    return percentage;
}

void IncomeTaxPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}