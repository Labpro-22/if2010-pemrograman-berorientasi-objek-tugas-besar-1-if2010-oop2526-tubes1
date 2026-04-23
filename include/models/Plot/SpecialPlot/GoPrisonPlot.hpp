#pragma once

#include "models/Plot/SpecialPlot/SpecialPlot.hpp"

class GoPrisonPlot : public SpecialPlot {
public:
    GoPrisonPlot(std::string name, std::string code);
    ~GoPrisonPlot() = default;

    std::string getType() const override;
    void startEvent() override;
};