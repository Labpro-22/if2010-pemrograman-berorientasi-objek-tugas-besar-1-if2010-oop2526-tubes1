// #pragma once

// #include <string>
// #include <vector>
// #include <memory>

// class Player;
// class Board;
// class PropertyTile;
// class LogEntry;

// enum class GameState; 

// class ViewGame {
// public:
//     ViewGame() = default;
//     ~ViewGame() = default;

//     void renderBoard(const Board& board) const; //kalo butuh list of player ambil dari gamemanager::players
//     void renderPlayerStats(const Player& player) const;
//     void renderPropertyList(const std::vector<PropertyTile*>& properties) const;
//     void renderMenu(GameState currentState, const std::string& playerName) const;
//     void showMessage(const std::string& message) const;
//     void renderLogs(const std::vector<LogEntry>& logs) const;
    
//     // Fungsi untuk mengambil satu baris perintah dari user (misal: "LEMPAR_DADU"). Fungsi ini akan membekukan program sampai user menekan ENTER.
//     std::string getInput() const;

//     bool getConfirmation() const;

//     int getNumericInput() const;
// };