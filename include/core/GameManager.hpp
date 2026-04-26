#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "../../include/core/CardManager.hpp"
#include "../../include/core/PropertyManager.hpp"
#include "../../include/core/EconomyManager.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/core/TransactionLog.hpp"

// State Machine
enum class GameState {
    START_TURN,              
    WAITING_FOR_COMMAND,     
    WAITING_FOR_DICE_ROLL,   
    LANDING_RESOLUTION,      
    WAITING_FOR_LIQUIDATION, 
    END_TURN,                
    AUCTION_BIDDING,
    GAME_OVER                
};

class GameManager {
private:
    int current_player_index; 
    int current_turn_count;   
    int max_turns;

    // Status permainan
    GameState current_state;

    // Map yang menghubungkan command dengan fungsi. Parameter string di dalamnya digunakan untuk menampung argumen tambahan (contoh: "500" dari "BID 500")
    std::unordered_map<std::string, std::function<void(const std::string&)>> command_map;

    // void setupCommands(); // Fungsi mendaftarkan isi commandMap

    // // Handler tiap command
    // void printBoard(const std::string& args);
    // void printCertificate(const std::string& args);     
    // void printProperty(const std::string& args);
    // void printLog(const std::string& args);     
    // void mortgage(const std::string& args);
    // void redeem(const std::string& args);
    // void build(const std::string& args);       
    // void useAbility(const std::string& args);
    // void rollDice(const std::string& args);
    // void setDice(const std::string& args);     
    // void bid(const std::string& args);       
    // void pass(const std::string& args);          
    // void save(const std::string& args); 
    // void load(const std::string& args);
    // void endTurn(const std::string& args);


    // std::shared_ptr<Player> getCurrentPlayer();
    // void nextPlayer();
    // void checkGameOver();

public:
static std::unique_ptr<CardManager> card_manager;
    static std::unique_ptr<PropertyManager> property_manager;
    static std::unique_ptr<EconomyManager> economy_manager;
    static std::unique_ptr<TransactionLog> logger;

    // Data pemain dan turn
    static std::vector<std::shared_ptr<Player>> players;
    GameManager(int maxTurns, std::vector<std::shared_ptr<Player>> initialPlayers, 
                std::unique_ptr<CardManager> cMgr, 
                std::unique_ptr<PropertyManager> pMgr, 
                std::unique_ptr<EconomyManager> eMgr,
                std::unique_ptr<TransactionLog> tLogger);
    ~GameManager() = default;

    // Siklus utama permainan
    void startGame();
    // void processCommand(const std::string& commandLine);    
    void displayBoard() const;    
    GameState getState() const { return current_state; }

    // Implementasi visitor tile

    static void visitCardTile(CardTile* tile, Player& player) ;
    static void visitTaxTile(TaxTile* tile, Player& player) ;
    static void visitFestivalTile(FestivalTile* tile, Player& player) ;
    static void visitGoTile(GoTile* tile, Player& player) ;
    static void visitGoToJailTile(GoToJailTile* tile, Player& player) ;
    static void visitFreeParkingTile(FreeParkingTile* tile, Player& player) ;
    static void visitJailTile(JailTile* tile, Player& player) ;
    static void visitStreetTile(StreetTile* tile, Player& player) ;
    static void visitRailroadTile(RailroadTile* tile, Player& player) ;
    static void visitUtilityTile(UtilityTile* tile, Player& player) ;


    // Implementasi save
    std::string toSaveFormat() const;
};