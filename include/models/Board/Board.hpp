#pragma once
#include <vector>
#include <string>
#include <memory>

#include "CardDeck.hpp"
#include "models/Plot/Plot.hpp"
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Card/CommunityChestCard.hpp"
#include "models/Card/SkillCard/SkillCard.hpp"

class Board {
public:
    static const int MIN_BOARD_SIZE = 20;
    static const int MAX_BOARD_SIZE = 60;
    static const int DEFAULT_BOARD_SIZE = 40;
 
private:
    vector<unique_ptr<Plot>> tiles;
 
    CardDeck<unique_ptr<ChanceCard>> chanceDeckPile;
    CardDeck<CommunityChestCard> communityChestDeckPile;
    CardDeck<unique_ptr<SkillCard>>  skillCardDeckPile;
 
    Board(const Board&)            = delete;
    Board& operator=(const Board&) = delete;
 
public:
    Board();
    ~Board();
 
    Board(Board&&) noexcept            = default;
    Board& operator=(Board&&) noexcept = default;
 
    bool addPlot(unique_ptr<Plot> plot);
    bool isReady() const;
 
    Plot* getPlot(int index) const;
    int getSize() const;
    const vector<unique_ptr<Plot>>& getPlots() const;
    
    int findPlotIndex(const string& code) const;
 
    CardDeck<unique_ptr<ChanceCard>>& getChanceDeckPile();
    CardDeck<CommunityChestCard>& getCommunityChestDeckPile();
    CardDeck<unique_ptr<SkillCard>>&   getSkillCardDeckPile();
 
    const CardDeck<unique_ptr<ChanceCard>>& getChanceDeckPile() const;
    const CardDeck<CommunityChestCard>& getCommunityChestDeckPile() const;
    const CardDeck<unique_ptr<SkillCard>>&  getSkillCardDeckPile() const;
 
    unique_ptr<ChanceCard> drawChanceCard();
    CommunityChestCard drawCommunityChestCard();
    unique_ptr<SkillCard> drawSkillCard();
};