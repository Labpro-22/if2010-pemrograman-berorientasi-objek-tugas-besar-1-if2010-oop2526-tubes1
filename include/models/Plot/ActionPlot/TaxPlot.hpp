#pragma once

#include "models/Plot/ActionPlot/ActionPlot.hpp"

class TaxPlot : public ActionPlot {
protected:
    int flatAmount;
    static int PPH, PBM;

public:
    TaxPlot(std::string name, std::string code, Color color);
    virtual ~TaxPlot() = default;

    PlotType getType() const override;
    int getFlatAmount(PlotContext& ctx) const;
};