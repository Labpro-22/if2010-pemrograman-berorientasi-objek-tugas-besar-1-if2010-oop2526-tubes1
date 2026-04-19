#pragma once

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include "../utils/GameException.hpp"

template <typename T>
class Deck {
private:
    std::vector<std::shared_ptr<T>> drawPile;
    std::vector<std::shared_ptr<T>> discardPile;

public:
    void addCard(const std::shared_ptr<T>& card) {
        if (card) {
            drawPile.push_back(card);
        }
    }

    void shuffle() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(drawPile.begin(), drawPile.end(), gen);
    }

    void refillFromDiscardIfNeeded() {
        if (!drawPile.empty() || discardPile.empty()) {
            return;
        }

        drawPile = discardPile;
        discardPile.clear();
        shuffle();
    }

    std::shared_ptr<T> draw() {
        refillFromDiscardIfNeeded();

        if (drawPile.empty()) {
            throw GameException("Deck is empty. Draw and discard piles are both empty.");
        }

        std::shared_ptr<T> card = drawPile.back();
        drawPile.pop_back();
        return card;
    }

    void discard(const std::shared_ptr<T>& card) {
        if (card) {
            discardPile.push_back(card);
        }
    }

    int size() const {
        return static_cast<int>(drawPile.size());
    }

    std::vector<std::shared_ptr<T>> getAllCards() const {
        std::vector<std::shared_ptr<T>> all = drawPile;
        all.insert(all.end(), discardPile.begin(), discardPile.end());
        return all;
    }

    const std::vector<std::shared_ptr<T>>& getDrawPile() const {
        return drawPile;
    }

    const std::vector<std::shared_ptr<T>>& getDiscardPile() const {
        return discardPile;
    }

    void clear() {
        drawPile.clear();
        discardPile.clear();
    }
};
