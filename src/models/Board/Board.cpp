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

CardDeck<unique_ptr<ChanceCard>>& Board::getChanceDeckPile() {
    return chanceDeckPile;
}
 
CardDeck<CommunityChestCard>& Board::getCommunityChestDeckPile() {
    return communityChestDeckPile;
}
 
CardDeck<unique_ptr<SkillCard>>& Board::getSkillCardDeckPile() {
    return skillCardDeckPile;
}
 
const CardDeck<unique_ptr<ChanceCard>>& Board::getChanceDeckPile() const {
    return chanceDeckPile;
}
 
const CardDeck<CommunityChestCard>& Board::getCommunityChestDeckPile() const {
    return communityChestDeckPile;
}
 
const CardDeck<unique_ptr<SkillCard>>& Board::getSkillCardDeckPile() const {
    return skillCardDeckPile;
}
 
unique_ptr<ChanceCard> Board::drawChanceCard() {
    return chanceDeckPile.draw();
}
 
CommunityChestCard Board::drawCommunityChestCard() {
    return communityChestDeckPile.draw();
}
 
unique_ptr<SkillCard> Board::drawSkillCard() {
    return skillCardDeckPile.draw();
}