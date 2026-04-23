#include "models/Plot/ActionPlot/CardPlot.hpp"

CardPlot::CardPlot(std::string name, std::string code, Color color)
    : ActionPlot(name, code, color) {}

std::string CardPlot::getType() const {
    return "Petak Kartu";
}