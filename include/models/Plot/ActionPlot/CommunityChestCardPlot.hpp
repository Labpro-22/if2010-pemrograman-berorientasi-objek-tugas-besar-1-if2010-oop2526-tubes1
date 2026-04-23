#pragma once
 
#include "models/Plot/ActionPlot/CardPlot.hpp"
 
class CommunityChestCardPlot : public CardPlot {
public:
    CommunityChestCardPlot(std::string name, std::string code);
    ~CommunityChestCardPlot() = default;
 
    std::string getType() const override;
    void startEvent() override;
};
 
