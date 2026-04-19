#pragma once
 
#include "models/Plot/ActionPlot/CardPlot.hpp"
 

class ChanceCardPlot : public CardPlot {
private:
    std::string lastDrawnCard;
 
public:
    ChanceCardPlot(std::string name, std::string code, CardDeck<std::string>& deck);
    ~ChanceCardPlot() = default;
 
    std::string getType() const override;
    void startEvent() override;
    std::string getLastDrawnCard() const;
};
 