#include "views/DisplayManager.hpp"

void DisplayManager::printBoard(GameMaster& gm) {
    // Clear screen dulu biar rapi
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    std::cout << boardRenderer.renderBoard(*gm.getGameState()->getBoard(), gm.getGameState()->getPlayers());
    
    // Tampilkan Legend di bawahnya
    std::cout << boardRenderer.renderLegend(gm.getGameState()->getPlayers()) << "\n";
}

void DisplayManager::printMessage(std::string msg) {
    std::cout << ">> MSG: " << msg << std::endl;
}