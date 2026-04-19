#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <string>
#include <memory>
#include "../Player/Player.hpp"     // Jordan
#include "../Board/Board.hpp"      // Yavie/Hakam
#include "../Card/CardDeck.hpp"   // Arin

class GameState {
private:
    int currTurn;
    int maxTurn;
    int activePlayerIdx;         // Indeks pemain yang sedang mendapat giliran

    std::vector<std::shared_ptr<Player>> turnOrder; // Menjamin urutan pemain tetap
    std::vector<std::shared_ptr<Player>> players;   // Daftar semua pemain
    std::unique_ptr<Board> board;                   // Status papan dan properti
    std::unique_ptr<CardDeck> chanceDeck;           // Tumpukan kartu Kesempatan
    std::unique_ptr<CardDeck> communityChestDeck;   // Tumpukan kartu Dana Umum

public:
    GameState();
    
    // Getter & Setter untuk Save/Load 
    int getCurrTurn() const;
    void setCurrTurn(int turn);

    int getMaxTurn() const;
    void setMaxTurn(int max);

    Player* getPlayer() const ;

    Player* getPlaterByid(int id) const;
    // Mengambil semua pemain untuk diserialisasi
    const std::vector<std::shared_ptr<Player>>& getPlayers() const;
    
    // Mengambil board untuk mengecek status properti
    Board* getGameBoard() const;

    // Mengambil deck untuk menyimpan urutan kartu
    CardDeck* getChanceDeck() const;
    CardDeck* getCommunityChestDeck() const;

    // Menentukan siapa yang sedang aktif
    int getActivePlayerIdx() const;
    void setActivePlayerIdx(int idx);

    const std::vector<std::shared_ptr<Player>>& getTurnOrder() const { return turnOrder; }
    int getActivePlayerIdx() const { return activePlayerIdx; }

    // Setter untuk Load
    void setTurnOrder(const std::vector<std::shared_ptr<Player>>& order) { turnOrder = order; }
    void setActivePlayerIdx(int idx) { activePlayerIdx = idx; }
};

#endif