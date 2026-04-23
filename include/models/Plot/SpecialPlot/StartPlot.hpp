#pragma once

#include "models/Plot/SpecialPlot/SpecialPlot.hpp"

class StartPlot : public SpecialPlot {
public:
    StartPlot(std::string name, std::string code);
    ~StartPlot() = default;

    std::string getType() const override;
    void startEvent() override;
};