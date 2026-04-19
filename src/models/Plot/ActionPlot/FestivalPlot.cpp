#include "models/Plot/ActionPlot/FestivalPlot.hpp"
#include <iostream>

FestivalPlot::FestivalPlot(std::string name, std::string code)
    : ActionPlot(name, code) {}

std::string FestivalPlot::getType() const {
    return "Petak Festival";
}

void FestivalPlot::startEvent() {
    std::cout << "Kamu mendarat di petak Festival!" << std::endl;
}