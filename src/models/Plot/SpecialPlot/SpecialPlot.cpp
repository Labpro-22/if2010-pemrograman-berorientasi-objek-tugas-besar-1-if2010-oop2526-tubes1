#include "models/Plot/SpecialPlot/SpecialPlot.hpp"

SpecialPlot::SpecialPlot(std::string name, std::string code)
    : Plot(name, code) {}

Color SpecialPlot::getColor() const {
    return Color::DEFAULT;
}

std::string SpecialPlot::getType() const {
    return "Petak Spesial";
}