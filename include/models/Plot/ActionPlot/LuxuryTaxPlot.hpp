#pragma once

#include "models/Plot/ActionPlot/TaxPlot.hpp"

class LuxuryTaxPlot : public TaxPlot {
public:
    LuxuryTaxPlot(std::string name, std::string code, Color color);
    ~LuxuryTaxPlot() = default;

    void startEvent(PlotContext& ctx) override;
};