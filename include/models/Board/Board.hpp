#pragma once
#include <vector>
#include <string>
#include <memory>

#include "CardDeck.hpp"
#include "models/Plot/Plot.hpp"
#include "models/Card/ChanceCard/ChanceCard.hpp"
#include "models/Card/CommunityChestCard/CommunityChestCard.hpp"
#include "models/Card/SkillCard/SkillCard.hpp"

class Board {
public:
    static const int MIN_BOARD_SIZE = 20;
    static const int MAX_BOARD_SIZE = 60;
    static const int DEFAULT_BOARD_SIZE = 40;
 
private:
    vector<unique_ptr<Plot>> tiles;
 
    CardDeck<ChanceCard*>         chanceDeckPile;
    CardDeck<CommunityChestCard*> communityChestDeckPile;
    CardDeck<SkillCard*>          skillCardDeckPile;

    vector<ChanceCard*> ownedChanceCards;
    vector<CommunityChestCard*> ownedCommunityChestCards;
    vector<SkillCard*> ownedSkillCards;
 
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
    bool isPlayerOwnAllColor(Color color, const Player* player) const;

    void initializeChanceDeck(std::vector<ChanceCard*> cards);
    void initializeCommunityChestDeck(std::vector<CommunityChestCard*> cards);
    void initializeSkillDeck(std::vector<SkillCard*> cards);
 
    CardDeck<ChanceCard*>& getChanceDeckPile();
    CardDeck<CommunityChestCard*>& getCommunityChestDeckPile();
    CardDeck<SkillCard*>& getSkillCardDeckPile();
 
    const CardDeck<ChanceCard*>& getChanceDeckPile() const;
    const CardDeck<CommunityChestCard*>& getCommunityChestDeckPile() const;
    const CardDeck<SkillCard*>& getSkillCardDeckPile() const;
 
    ChanceCard* drawChanceCard();
    CommunityChestCard* drawCommunityChestCard();
    SkillCard* drawSkillCard();
};
