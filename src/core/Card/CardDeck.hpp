#ifndef CARDDECK_HPP
#define CARDECK_HPP

using namespace std;

template <class T>
class CardDeck
{
private:
    vector<T> drawPile;
    vector<T> discardPile;

public:
    CardDeck();
    ~CardDeck();
    void setCards();
    bool isEmpty();
    vector getDrawPile();
    vector getDiscardPile();
    T *draw();
    void pushToDrawPile();
    void discard(T);
    void shuffle();
    void reshuffle();
    size_t drawPileSize();
    size_t discardPileSize();
}

#endif