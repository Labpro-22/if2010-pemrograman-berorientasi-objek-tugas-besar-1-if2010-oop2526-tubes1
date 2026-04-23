#pragma once

#include "models/Plot/ActionPlot/TaxPlot.hpp"

class IncomeTaxPlot : public TaxPlot {
private:
    int percentage;

public:
    IncomeTaxPlot(std::string name, std::string code, int flatAmount, int percentage);
    ~IncomeTaxPlot() = default;

    std::string getType() const override;
    int getPercentage() const;
    void startEvent() override;
};