#include "models/Plot/ActionPlot/CommunityChestCardPlot.hpp"

CommunityChestCardPlot::CommunityChestCardPlot(std::string name, std::string code, Color color)
    : CardPlot(name, code, color) {}

std::string CommunityChestCardPlot::getType() const {
    return "Petak Dana Umum";
}

void CommunityChestCardPlot::startEvent(PlotContext& ctx) {
    //TODO (PlotContext)
}