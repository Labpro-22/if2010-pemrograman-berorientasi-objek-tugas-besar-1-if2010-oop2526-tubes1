#pragma once

#include "models/Plot/ActionPlot/CardPlot.hpp"

class CommunityChestCardPlot : public CardPlot {
private:
    std::string lastDrawnCard;

public:
    CommunityChestCardPlot(std::string name, std::string code, CardDeck<std::string>& deck);
    ~CommunityChestCardPlot() = default;

    std::string getType() const override;
    void startEvent() override;
    std::string getLastDrawnCard() const;
};