#pragma once

#include "models/Plot/SpecialPlot/SpecialPlot.hpp"

class FreeParkPlot : public SpecialPlot {
public:
    FreeParkPlot(std::string name, std::string code);
    ~FreeParkPlot() = default;

    std::string getType() const override;
    void startEvent() override;
};