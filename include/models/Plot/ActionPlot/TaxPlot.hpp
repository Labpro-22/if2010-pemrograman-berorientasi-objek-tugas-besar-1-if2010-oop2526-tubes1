#pragma once

#pragma once

#include "models/Plot/ActionPlot/ActionPlot.hpp"

enum class TaxType {
    PPH, 
    PBM   
};

class TaxPlot : public ActionPlot {
private:
    TaxType taxType;
    int flatAmount;      // PPH flat atau PBM 
    int percentage;      // PPH percentage (kalau PBM nilainya 0)

public:
    TaxPlot(std::string name, std::string code, TaxType taxType, int flatAmount, int percentage = 0);
    ~TaxPlot() = default;

    std::string getType() const override;

    TaxType getTaxType() const;
    int getFlatAmount() const;
    int getPercentage() const;
    void startEvent(PlotContext& ctx) override;
};