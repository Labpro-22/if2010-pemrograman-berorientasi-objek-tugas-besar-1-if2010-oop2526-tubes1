#pragma once

#include "models/Plot/Plot.hpp"

class ActionPlot : public Plot {
public:
    ActionPlot(std::string name, std::string code, Color color);
    virtual ~ActionPlot() = default;
    
    std::string getType() const override;
};