#pragma once

#include "models/Plot/SpecialPlot/SpecialPlot.hpp"

class FreeParkPlot : public SpecialPlot {
public:
    FreeParkPlot(std::string name, std::string code, Color color);
    ~FreeParkPlot() = default;

    std::string getType() const override;
    void startEvent(PlotContext& ctx) override;
};