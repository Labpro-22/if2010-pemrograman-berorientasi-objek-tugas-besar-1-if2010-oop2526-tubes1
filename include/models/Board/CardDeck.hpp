#pragma once
#include "iostream"
#include "deque"
#include "vector"
#include "random"
#include "algorithm"
using namespace std;

template<class T>
class CardDeck {
    private:
        deque<T> drawPile;
        deque<T> discardPile;
    public:
        CardDeck();
        ~CardDeck();

        void shuffle();
        void initialize(vector<T>);

        T draw();
        void discard(T);

        int drawPileSize();
        int discardPileSize();

        vector<T> getAllCards();
};

