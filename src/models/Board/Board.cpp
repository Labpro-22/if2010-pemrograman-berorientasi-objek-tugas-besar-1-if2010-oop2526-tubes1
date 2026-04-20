#include "models/Board/Board.hpp"

static const int BOARD_SIZE   = 40;
static const int TOP_START    = 0;
static const int TOP_END      = 10;
static const int RIGHT_START  = 11;
static const int RIGHT_END    = 19;
static const int BOTTOM_START = 20;
static const int BOTTOM_END   = 30;
static const int LEFT_START   = 31;
static const int LEFT_END     = 39;
static const int CELL_WIDTH   = 5;

Board::Board() {
    this->size = 40;
    this->chanceDeckPile = new CardDeck<ChanceCard>();
    this->communityChestDeckPile = new CardDeck<CommunityChestCard>();
    this->skillCardDeckPile = new CardDeck<SkillCard>();
}

Board::~Board() {
    for (auto it = tiles.begin(); it != tiles.end(); ++it) {
        delete *it;
    }
    tiles.clear();

    delete chanceDeckPile;
    delete communityChestDeckPile;
    delete skillCardDeckPile;
}

ChanceCard Board::drawChanceCard() {
    return chanceDeckPile->draw();
}

CommunityChestCard Board::drawCommunityChestCard() {
    return communityChestDeckPile->draw();
}

SkillCard Board::drawSkillCard() {
    return skillCardDeckPile->draw();
}

vector<Plot*> Board::getPlots() const {
    return tiles;
}

Plot* Board::getPlot(int index) const {
    if (index >= 0 && index < size) {
        return tiles[index];
    }
    return nullptr;
}

int Board::getSize() const {
    return size;
}

CardDeck<ChanceCard>* Board::getChanceDeckPile() const {
    return chanceDeckPile;
}

CardDeck<CommunityChestCard>* Board::getCommunityChestDeckPile() const {
    return communityChestDeckPile;
}

CardDeck<SkillCard>* Board::getSkillCardDeckPile() const {
    return skillCardDeckPile;
}

static string formatCell(const string& code) {
    string c = code.substr(0, CELL_WIDTH);
    c += string(CELL_WIDTH - c.length(), ' ');
    return "[" + c + "]";
}

void Board::printBoard() {
    if (tiles.empty()) {
        cout << "[Board] Papan belum diinisialisasi." << endl;
        return;
    }

    using Iter = vector<Plot*>::const_iterator;
    using ReverseIter = vector<Plot*>::const_reverse_iterator;

    Iter topBegin = tiles.cbegin() + TOP_START;
    Iter topEnd = tiles.cbegin() + TOP_END + 1;

    for (Iter it = topBegin; it != topEnd; ++it) {
        cout << formatCell((*it)->getCode());
    }
    cout << endl;

    ReverseIter leftIt = tiles.crbegin();
    ReverseIter leftEnd = tiles.crbegin() + (BOARD_SIZE - LEFT_START);

    Iter rightIt = tiles.cbegin() + RIGHT_START;
    Iter rightEnd = tiles.cbegin() + RIGHT_END + 1;

    const int innerCols = (TOP_END - TOP_START) - 1;

    for (; leftIt != leftEnd && rightIt != rightEnd; ++leftIt, ++rightIt) {
        cout << formatCell((*leftIt)->getCode());
        for (int i = 0; i < innerCols; ++i) {
            std::cout << std::string(CELL_WIDTH + 2, ' ');
        }
        cout << formatCell((*rightIt)->getCode()) << endl;
    }

    ReverseIter botBegin = tiles.crbegin() + (BOARD_SIZE - 1 - BOTTOM_END);
    ReverseIter botEnd = tiles.crbegin() + (BOARD_SIZE - BOTTOM_START);

    for (ReverseIter it = botBegin; it != botEnd; ++it) {
        cout << formatCell((*it)->getCode());
    }
    cout << endl;
}