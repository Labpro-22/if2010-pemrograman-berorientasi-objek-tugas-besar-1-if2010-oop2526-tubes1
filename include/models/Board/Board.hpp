#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "CardDeck.hpp"
#include "../Plot/Plot.hpp"
#include "../Card/ChanceCard.hpp"
#include "../Card/CommunityChestCard.hpp"
#include "../Card/SkillCard/SkillCard.hpp"
using namespace std;
class Board {
private:
    vector<Plot*> tiles;
    int size;

    CardDeck<ChanceCard>* chanceDeckPile;
    CardDeck<CommunityChestCard>* communityChestDeckPile;
    CardDeck<SkillCard>* skillCardDeckPile;

public:
    Board();
    ~Board();
    
    void printBoard();

    ChanceCard drawChanceCard();
    CommunityChestCard drawCommunityChestCard();
    SkillCard drawSkillCard();

    vector<Plot*> getPlots() const;
    Plot* getPlot(int index) const;
    int getSize() const;

    CardDeck<ChanceCard>* getChanceDeckPile() const;
    CardDeck<CommunityChestCard>* getCommunityChestDeckPile() const;
    CardDeck<SkillCard>* getSkillCardDeckPile() const;

};
