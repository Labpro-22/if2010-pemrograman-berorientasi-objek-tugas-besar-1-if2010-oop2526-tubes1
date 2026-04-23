#pragma once

#include "models/Plot/ActionPlot/CardPlot.hpp"

class ChanceCardPlot : public CardPlot {
public:
    ChanceCardPlot(std::string name, std::string code);
    ~ChanceCardPlot() = default;

    std::string getType() const override;
    void startEvent() override;
};
