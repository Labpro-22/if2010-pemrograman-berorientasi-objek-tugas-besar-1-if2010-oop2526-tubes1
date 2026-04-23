#include "models/Plot/SpecialPlot/FreeParkPlot.hpp"
#include <iostream>

FreeParkPlot::FreeParkPlot(std::string name, std::string code)
    : SpecialPlot(name, code) {}

std::string FreeParkPlot::getType() const {
    return "Petak Bebas Parkir";
}

void FreeParkPlot::startEvent() {
    //TODO (PlotContext)
}