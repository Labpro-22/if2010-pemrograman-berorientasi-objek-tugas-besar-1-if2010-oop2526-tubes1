#include "models/Plot/ActionPlot/ChanceCardPlot.hpp"
#include <iostream>

ChanceCardPlot::ChanceCardPlot(std::string name, std::string code, CardDeck<std::string>& deck)
    : CardPlot(name, code, deck), lastDrawnCard("") {}

std::string ChanceCardPlot::getType() const {
    return "Petak Kesempatan";
}

void ChanceCardPlot::startEvent() {
    std::cout << "Kamu mendarat di Petak Kesempatan!" << std::endl;
    std::cout << "Mengambil kartu..." << std::endl;

    lastDrawnCard = drawCard();
    deck.discard(lastDrawnCard);

    std::cout << "Kartu: \"" << lastDrawnCard << "\"" << std::endl;
}

std::string ChanceCardPlot::getLastDrawnCard() const {
    return lastDrawnCard;
}