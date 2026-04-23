#pragma once

#include "models/Plot/ActionPlot/TaxPlot.hpp"

class IncomeTaxPlot : public TaxPlot {
private:
    int percentage;

public:
    IncomeTaxPlot(std::string name, std::string code, Color color);
    ~IncomeTaxPlot() = default;

    std::string getType() const override;
    int getPercentage() const;
    void startEvent(PlotContext& ctx) override;
};