#include "models/Board/CardDeck.hpp"

template<class T>
CardDeck<T>::CardDeck() {
    this->drawPile = {};
    this->discardPile = {};
}

template<class T>
CardDeck<T>::~CardDeck() {

}

template<class T>
void CardDeck<T>::shuffle() {
    random_device random;
    mt19937 g(random);

    shuffle(drawPile.begin(), drawPile.end(), g);
}

template<class T>
void CardDeck<T>::initialize(vector<T> card) {
    for(auto it = card.begin(); it < card.end(); ++it) {
        drawPile.push_back(it);
    }
}

template<class T>
T CardDeck<T>::draw() {
    return drawPile.back();
}

template<class T>
void CardDeck<T>::discard(T card) {
    for(auto it = drawPile.end(); it != drawPile.begin(); it--) {
        if(it == card) {
            drawPile.erase(it);
            discardPile.push_back(it);
            break;
        }
    }
}

template<class T>
int CardDeck<T>::drawPileSize() {
    return drawPile.size();
}

template<class T>
int CardDeck<T>::discardPileSize() {
    return discardPile.size();
}

template<class T>
vector<T> CardDeck<T>::getAllCards() {
    vector<T> deck;
    for(auto it = drawPile.begin(); it < drawPile.end(); ++it) {
        deck.push_back(it);
    }
}

