#pragma once

#include <utility>
#include <vector>

template <typename T>
class CardDeck {
private:
	std::vector<T> availableCards;
	std::vector<T> usedCard;

public:
	CardDeck() = default;
	CardDeck(CardDeck&&) noexcept = default;
	CardDeck& operator=(CardDeck&&) noexcept = default;
	virtual ~CardDeck() = default;

	void shuffleDeck();
	void printDeck() const;
	T topDeck();
	T* getRandomCard();
};
