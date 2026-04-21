#pragma once

#include <string>
#include <vector>

#include "Board.hpp"
#include "Card.hpp"
#include "Deck.hpp"
#include "Logger.hpp"

class Player;
class Tile;

class GameManager {
private:
	int turn;
	int maxTurn;
	int activePlayerCount;
	int playerCount;
	std::vector<Player*> players;
	Board board;
	CardDeck<SkillCard> deckSkill;
	CardDeck<AutoUseCard> deckChance;
	CardDeck<AutoUseCard> deckCurrency;
    Logger logger;
	Player* currentTurnPlayer;
	static std::vector<int> dice;

public:
	GameManager();
	static GameManager& getInstance() {
		static GameManager instance;
		return instance;
	}

	bool isGameValid();
	void runGame();
	void auction(Tile*);
	void initBoard();
	void initPlayers();
	void initStateLogs();
	void initSkillDeck();
	Board& getBoard() {
		return board;
	}
	const Board& getBoard() const {
		return board;
	}
    Logger& getLogger();
};
