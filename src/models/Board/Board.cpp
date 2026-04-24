#include "models/Board/Board.hpp"
 
Board::Board()
    : tiles(),
      chanceDeckPile(),
      communityChestDeckPile(),
      skillCardDeckPile()
{
    tiles.reserve(DEFAULT_BOARD_SIZE);
}
 
Board::~Board() {
    for (ChanceCard* c : ownedChanceCards) delete c;
    for (CommunityChestCard* c : ownedCommunityChestCards) delete c;
    for (SkillCard* c : ownedSkillCards) delete c;
}
 
bool Board::addPlot(unique_ptr<Plot> plot) {
    if (!plot) return false;
    if (static_cast<int>(tiles.size()) >= MAX_BOARD_SIZE) {
        cerr << "[Board] Papan sudah penuh (" << MAX_BOARD_SIZE
                  << " petak). Plot \"" << plot->getCode()
                  << "\" tidak ditambahkan." << endl;
        return false;
    }
    tiles.push_back(move(plot));
    return true;
}
 
bool Board::isReady() const {
    int size = static_cast<int>(tiles.size());
    return size >= MIN_BOARD_SIZE && size <= MAX_BOARD_SIZE;
}
 
Plot* Board::getPlot(int index) const {
    if (index < 0 || index >= static_cast<int>(tiles.size())) {
        return nullptr;
    }
    return tiles[index].get(); 
}
 
int Board::getSize() const {
    return static_cast<int>(tiles.size());
}
 
const vector<unique_ptr<Plot>>& Board::getPlots() const {
    return tiles;
}
 
int Board::findPlotIndex(const string& code) const {
    for (int i = 0; i < static_cast<int>(tiles.size()); ++i) {
        if (tiles[i] && tiles[i]->getCode() == code) {
            return i;
        }
    }
    return -1;
}

void Board::initializeChanceDeck(std::vector<ChanceCard*> cards) {
    ownedChanceCards = cards;  
    chanceDeckPile.initialize(cards);
    chanceDeckPile.shuffle();
}
 
void Board::initializeCommunityChestDeck(std::vector<CommunityChestCard*> cards) {
    ownedCommunityChestCards = cards;
    communityChestDeckPile.initialize(cards);
    communityChestDeckPile.shuffle();
}
 
void Board::initializeSkillDeck(std::vector<SkillCard*> cards) {
    ownedSkillCards = cards;
    skillCardDeckPile.initialize(cards);
    skillCardDeckPile.shuffle();
}

CardDeck<ChanceCard*>& Board::getChanceDeckPile() {
    return chanceDeckPile;
}
CardDeck<CommunityChestCard*>& Board::getCommunityChestDeckPile() {
    return communityChestDeckPile;
}
CardDeck<SkillCard*>& Board::getSkillCardDeckPile() {
    return skillCardDeckPile;
}
 
const CardDeck<ChanceCard*>& Board::getChanceDeckPile() const {
    return chanceDeckPile;
}
const CardDeck<CommunityChestCard*>& Board::getCommunityChestDeckPile() const {
    return communityChestDeckPile;
}
const CardDeck<SkillCard*>& Board::getSkillCardDeckPile() const {
    return skillCardDeckPile;
}
 
ChanceCard* Board::drawChanceCard() {
    return chanceDeckPile.draw();
}
 
CommunityChestCard* Board::drawCommunityChestCard() {
    return communityChestDeckPile.draw();
}
 
SkillCard* Board::drawSkillCard() {
    return skillCardDeckPile.draw();
}

