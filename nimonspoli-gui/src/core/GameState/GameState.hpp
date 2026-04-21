#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <string>
#include <memory>
#include "../Card/CardDeck.hpp"

// Forward declarations — hindari circular dependency
class Player;
class Board;
class Card;

class GameState {
private:
    int currTurn;
    int maxTurn;
    int activePlayerIdx;

    std::vector<std::shared_ptr<Player>> turnOrder;
    std::vector<std::shared_ptr<Player>> players;
    std::unique_ptr<Board> board;
    std::unique_ptr<CardDeck<Card>> chanceDeck;
    std::unique_ptr<CardDeck<Card>> communityChestDeck;

public:
    GameState(int currTurn,
                int maxTurn,
                int activePlayerIdx,
                std::vector<std::shared_ptr<Player>> turnOrder,
                std::vector<std::shared_ptr<Player>> players,
                std::unique_ptr<Board> board,
                std::unique_ptr<CardDeck<Card>> chanceDeck,
                std::unique_ptr<CardDeck<Card>> communityChestDeck
            );

    int getCurrTurn() const;
    void setCurrTurn(int turn);
    int getMaxTurn() const;
    void setMaxTurn(int max);
    Player* getPlayer() const;
    Player* getPlaterByid(int id) const;
    const std::vector<std::shared_ptr<Player>>& getPlayers() const;
    Board* getGameBoard() const;
    CardDeck<Card>* getChanceDeck() const;
    CardDeck<Card>* getCommunityChestDeck() const;

    // Tidak duplikat — hanya deklarasi + inline di sini
    int getActivePlayerIdx() const { return activePlayerIdx; }
    void setActivePlayerIdx(int idx) { activePlayerIdx = idx; }
    const std::vector<std::shared_ptr<Player>>& getTurnOrder() const { return turnOrder; }
    void setTurnOrder(const std::vector<std::shared_ptr<Player>>& order) { turnOrder = order; }
};

#endif