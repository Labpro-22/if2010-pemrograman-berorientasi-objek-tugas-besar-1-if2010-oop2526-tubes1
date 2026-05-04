#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <vector>
#include <string>
#include <random>

// Forward Declarations - Hindari include di header untuk mencegah circular dependency
class Player;
class Board;
class SaveLoadManager;
class TransactionLogger;
class Auction;
class SkillCard; 
template <typename T> class CardDeck; 

class GameEngine {
private:
    // State Game
    std::vector<Player*> players;
    Board* board;
    SaveLoadManager* saveLoadManager;
    TransactionLogger* logger;
    CardDeck<SkillCard>* skillDeck; // Sesuaikan dengan tipe dari Miguel
    
    int currentTurnIdx; // Indeks pemain aktif (0-3)
    int roundCount;     // Jumlah putaran yang sudah berjalan
    bool diceRolled;
    bool turnEnded;

    // Utilitas Internal
    void checkWinCondition();
    void nextTurn();

public:
    GameEngine(Board *board, TransactionLogger *logger);
    ~GameEngine();

    // Kontrol Alur Permainan
    void startNewGame(const std::vector<std::string>& playerNames);
    void loadGame(const std::string& filePath);
    void saveGame(const std::string& filePath);
    
    // Logika Turn
    void rollDice(int d1 = 0, int d2 = 0); // Support ATUR_DADU (overload)
    void executeTileAction();
    void endTurn();

    // Getters untuk Integrasi (Dibutuhkan SaveLoadManager & Auction)
    Player* getCurrentPlayer() const;
    std::vector<Player*> getAllPlayers() const { return players; }
    Board* getBoard() const { return board; }
    int getCurrentRound() const { return roundCount; }
    int getCurrentTurnIdx() const { return currentTurnIdx; }
    
    // Flags
    bool hasDiceRolled() const { return diceRolled; }
    bool isTurnEnded() const { return turnEnded; }
};

#endif