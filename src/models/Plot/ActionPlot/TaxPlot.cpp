#include "models/Plot/ActionPlot/TaxPlot.hpp"

TaxPlot::TaxPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color) {}

std::string TaxPlot::getType() const {
    return "Petak Pajak";
}

int TaxPlot::getFlatAmount(PlotContext& ctx) const {
    return FLAT;
}

void TaxPlot::setFlatAmount(int amount){
    FLAT = amount;
}

void TaxPlot::setPPH(int amount){
    PPH = amount;
}
void TaxPlot::setPBM(int amount){
    PBM = amount;
}