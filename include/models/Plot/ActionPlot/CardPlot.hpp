#pragma once

#include "models/Plot/ActionPlot/ActionPlot.hpp"


class CardPlot : public ActionPlot {
public:
    CardPlot(std::string name, std::string code, Color color);
    virtual ~CardPlot() = default;

    std::string getType() const override;
};