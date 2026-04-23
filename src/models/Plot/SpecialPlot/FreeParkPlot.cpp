#include "models/Plot/SpecialPlot/FreeParkPlot.hpp"
#include <iostream>

FreeParkPlot::FreeParkPlot(std::string name, std::string code, Color color)
    : SpecialPlot(name, code, color) {}

std::string FreeParkPlot::getType() const {
    return "Petak Bebas Parkir";
}

void FreeParkPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}