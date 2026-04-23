#pragma once

#include "models/Plot/ActionPlot/ActionPlot.hpp"

class TaxPlot : public ActionPlot {
protected:
    static int FLAT, PPH, PBM;

public:
    TaxPlot(std::string name, std::string code, Color color);
    virtual ~TaxPlot() = default;

    std::string getType() const override;
    int getFlatAmount(PlotContext& ctx) const;
    static void setFlatAmount(int amount);
    static void setPPH(int amount);
    static void setPBM(int amount);
};