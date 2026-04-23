#include "models/Plot/ActionPlot/LuxuryTaxPlot.hpp"

LuxuryTaxPlot::LuxuryTaxPlot(std::string name, std::string code, Color color)
    : TaxPlot(name, code, color) {}

std::string LuxuryTaxPlot::getType() const {
    return "Petak Pajak Barang Mewah";
}

void LuxuryTaxPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}