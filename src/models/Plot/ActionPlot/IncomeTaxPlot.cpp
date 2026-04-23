#include "models/Plot/ActionPlot/IncomeTaxPlot.hpp"

IncomeTaxPlot::IncomeTaxPlot(std::string name, std::string code,
                             int flatAmount, int percentage)
    : TaxPlot(name, code, flatAmount), percentage(percentage) {}

std::string IncomeTaxPlot::getType() const {
    return "Petak Pajak Penghasilan";
}

int IncomeTaxPlot::getPercentage() const {
    return percentage;
}

void IncomeTaxPlot::startEvent() {
    //TODO (PlotContext)
}