#include "models/Plot/ActionPlot/ActionPlot.hpp"

ActionPlot::ActionPlot(std::string name, std::string code)
    : Plot(name, code), isActionPlot(true) {}

Color ActionPlot::getColor() const {
    return Color::DEFAULT;
}

std::string ActionPlot::getType() const {
    return "Petak Aksi";
}

bool ActionPlot::getIsActionPlot() const {
    return isActionPlot;
}