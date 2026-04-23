#pragma once

#include "models/Plot/Plot.hpp"

class SpecialPlot : public Plot {
public:
    SpecialPlot(std::string name, std::string code);
    virtual ~SpecialPlot() = default;

    Color getColor() const override;
    std::string getType() const override;
    virtual void startEvent() override = 0;
};