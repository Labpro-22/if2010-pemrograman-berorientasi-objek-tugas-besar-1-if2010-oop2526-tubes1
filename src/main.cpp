#include "../include/views/CommandProcessor.hpp"
#include "../include/core/GameController.hpp"
#include "../include/views/BoardView.hpp"
#include "../include/views/PropertyView.hpp"
#include <iostream>

using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "       MONOPOLI GAME - OOP Edition      " << endl;
    cout << "========================================" << endl;
    cout << endl;

    // Layer: Models - Initialize GameController
    GameController gameController;
    
    // Layer: UI - Initialize Views
    BoardView boardView;
    PropertyView propertyView;
    
    // Layer: CommandProcessor - Bridge between UI and GameController
    CommandProcessor commandProcessor(&gameController, &boardView, &propertyView);
    
    // Initialize game
    gameController.startGame();
    gameController.addPlayer("Player 1", 2000);
    gameController.addPlayer("Player 2", 2000);
    gameController.addPlayer("Player 3", 2000);
    
    cout << "\n[INFO] Ketik HELP untuk melihat command yang tersedia." << endl;
    cout << "[INFO] Mulai dengan CETAK_PAPAN untuk melihat board." << endl;
    cout << endl;
    
    // Main game loop
    while (true) {
        commandProcessor.readCommand();
    }

    return 0;
}
