#include "models/Plot/ActionPlot/CommunityChestCardPlot.hpp"

CommunityChestCardPlot::CommunityChestCardPlot(std::string name, std::string code)
    : CardPlot(name, code) {}

std::string CommunityChestCardPlot::getType() const {
    return "Petak Dana Umum";
}

void CommunityChestCardPlot::startEvent() {
    //TODO (PlotContext)
}