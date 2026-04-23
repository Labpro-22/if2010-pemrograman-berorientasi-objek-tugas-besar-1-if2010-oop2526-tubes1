#include "models/Plot/ActionPlot/CommunityChestCardPlot.hpp"
#include <iostream>

CommunityChestCardPlot::CommunityChestCardPlot(std::string name, std::string code, CardDeck<std::string>& deck)
    : CardPlot(name, code, deck), lastDrawnCard("") {}

std::string CommunityChestCardPlot::getType() const {
    return "Petak Dana Umum";
}

void CommunityChestCardPlot::startEvent(PlotContext& ctx) {
    std::cout << "Kamu mendarat di Petak Dana Umum!" << std::endl;
    std::cout << "Mengambil kartu..." << std::endl;

    lastDrawnCard = drawCard();

    deck.discard(lastDrawnCard);

    std::cout << "Kartu: \"" << lastDrawnCard << "\"" << std::endl;
}

std::string CommunityChestCardPlot::getLastDrawnCard() const {
    return lastDrawnCard;
}