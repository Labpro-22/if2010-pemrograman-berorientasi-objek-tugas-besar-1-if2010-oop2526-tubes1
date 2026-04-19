#pragma once
 
#include "models/Plot/ActionPlot/ActionPlot.hpp"
#include "models/Board/CardDeck.hpp"
#include <string>
 
class CardPlot : public ActionPlot {
protected:
    CardDeck<std::string>& deck;
 
public:
    CardPlot(std::string name, std::string code, CardDeck<std::string>& deck);
    virtual ~CardPlot() = default;
    std::string getType() const override;
    std::string drawCard();
};
 