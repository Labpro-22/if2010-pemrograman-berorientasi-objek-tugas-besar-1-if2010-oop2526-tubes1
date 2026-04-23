#include "models/Plot/ActionPlot/LuxuryTaxPlot.hpp"

LuxuryTaxPlot::LuxuryTaxPlot(std::string name, std::string code, int flatAmount)
    : TaxPlot(name, code, flatAmount) {}

std::string LuxuryTaxPlot::getType() const {
    return "Petak Pajak Barang Mewah";
}

void LuxuryTaxPlot::startEvent() {
    //TODO (PlotContext)
}