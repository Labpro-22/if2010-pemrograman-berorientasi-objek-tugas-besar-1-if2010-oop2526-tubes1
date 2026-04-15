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
    mt19937 g(random());

    std::shuffle(drawPile.begin(), drawPile.end(), g);
}

template<class T>
void CardDeck<T>::initialize(vector<T> card) {
    for(auto it = card.begin(); it < card.end(); ++it) {
        drawPile.push_back(*it);
    }

    discardPile.clear();
}

template<class T>
T CardDeck<T>::draw() {
    if(!drawPile.empty()) {
        T drawCard = drawPile.back();
        drawPile.pop_back();
        return drawCard;
    }
}

template<class T>
void CardDeck<T>::discard(T card) {
    reverse(drawPile.begin(), drawPile.end());
    auto it = find(drawPile.begin(), drawPile.end(), card);
    if(it != drawPile.end()) {
        discardPile.push_back(*it);
        drawPile.erase(it);
    }
    reverse(drawPile.begin(), drawPile.end());
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
        deck.push_back(*it);
    }
    return deck;
}

