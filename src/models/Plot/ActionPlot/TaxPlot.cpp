#include "models/Plot/ActionPlot/TaxPlot.hpp"

TaxPlot::TaxPlot(std::string name, std::string code, int flatAmount)
    : ActionPlot(name, code), flatAmount(flatAmount) {}

std::string TaxPlot::getType() const {
    return "Petak Pajak";
}

int TaxPlot::getFlatAmount() const {
    return flatAmount;
}
