#include "models/Plot/ActionPlot/CardPlot.hpp"

CardPlot::CardPlot(std::string name, std::string code)
    : ActionPlot(name, code) {}

std::string CardPlot::getType() const {
    return "Petak Kartu";
}