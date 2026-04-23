#include "models/Plot/SpecialPlot/SpecialPlot.hpp"

SpecialPlot::SpecialPlot(std::string name, std::string code, Color color)
    : Plot(name, code, color) {}

PlotType SpecialPlot::getType() const {
    return PlotType::SPECIALPLOT;
}