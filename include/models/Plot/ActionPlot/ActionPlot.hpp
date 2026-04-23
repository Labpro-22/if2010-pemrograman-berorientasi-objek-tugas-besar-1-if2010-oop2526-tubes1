#pragma once

#include "models/Plot/Plot.hpp"

class ActionPlot : public Plot {
public:
    ActionPlot(std::string name, std::string code);
    virtual ~ActionPlot() = default;
    
    Color getColor() const override;
    std::string getType() const override;
};