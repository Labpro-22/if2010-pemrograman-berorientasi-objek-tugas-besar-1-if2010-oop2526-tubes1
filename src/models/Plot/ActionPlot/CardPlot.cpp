#include "models/Plot/ActionPlot/CardPlot.hpp"
 
CardPlot::CardPlot(std::string name, std::string code, CardDeck<std::string>& deck)
    : ActionPlot(name, code), deck(deck) {}
 
std::string CardPlot::getType() const {
    return "Petak Kartu";
}
 
std::string CardPlot::drawCard() {
    return deck.draw();
}
 