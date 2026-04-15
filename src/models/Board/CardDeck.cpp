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

}

template<class T>
void CardDeck<T>::initialize(vector<T> card) {

}

template<class T>
T CardDeck<T>::draw() {

}

template<class T>
void CardDeck<T>::discard(T card) {

}

template<class T>
int CardDeck<T>::drawPileSize() {

}

template<class T>
int CardDeck<T>::discardPileSize() {

}

template<class T>
vector<T> CardDeck<T>::getAllCards() {

}

