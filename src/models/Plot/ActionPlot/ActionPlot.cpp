#include "models/Plot/ActionPlot/ActionPlot.hpp"

ActionPlot::ActionPlot(std::string name, std::string code, Color color)
    : Plot(name, code, color) {}

std::string ActionPlot::getType() const {
    return "Petak Aksi";
}
