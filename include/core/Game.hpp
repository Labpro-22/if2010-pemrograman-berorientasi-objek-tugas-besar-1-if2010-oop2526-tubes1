#pragma once

#include <vector>
#include <string>

class Board;
class Player;
class TurnManager;
class Dice;
class CardManager;
class AuctionManager;
class BankruptcyManager;
class TradeManager;
class LiquidationManager;
class Logger;
class Config;

using namespace std;

class Game {
private:
    Board board;
    vector<Player> players;
    TurnManager turnManager;
    Dice dice;
    CardManager cardManager;
    AuctionManager auctionManager;
    BankruptcyManager bankruptcyManager;
    TradeManager tradeManager;
    LiquidationManager liquidationManager;
    Logger logger;
    Config config;
    bool gameOver;
    
public:
    // Constructor
    Game();
    
    // Destructor
    ~Game() = default;

    // Core Game Loop Methods
    void run();
    void startTurn();
    void endTurn();
    void handleLanding(Player& player); 
    void checkWinCondition();
    void executeCommand(const std::string& command);

    // Getters / Setters    
    Board& getBoard();
    vector<Player>& getPlayers();
    Player& getPlayer(int index);
    TurnManager& getTurnManager();
    Dice& getDice();
    CardManager& getCardManager();
    AuctionManager& getAuctionManager();
    BankruptcyManager& getBankruptcyManager();
    TradeManager& getTradeManager();
    LiquidationManager& getLiquidationManager();
    Logger& getLogger();
    Config& getConfig();

    bool isGameOver() const;
    void setGameOver(bool status);
};