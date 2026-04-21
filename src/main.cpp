#include "../include/core/DataManager.hpp"
#include "../include/core/GameManager.hpp"

#include <exception>
#include <iostream>

int main() {
    try {
        // Get GameManager singleton instance
        GameManager& gm = GameManager::getInstance();
        
        // Set config paths
        std::string defaultConfigPath = "config/";
        DataManager dm(
            defaultConfigPath + "misc.txt",
            defaultConfigPath + "property.txt",
            defaultConfigPath + "tax.txt",
            defaultConfigPath + "utility.txt",
            defaultConfigPath + "railroad.txt",
            defaultConfigPath + "special.txt"
        );

        // Load all game data (tiles, players, config) to board via GameManager
        std::cout << "[INFO] Loading game data..." << std::endl;
        dm.load(gm);
        std::cout << "[SUCCESS] Game data loaded successfully!" << std::endl;
        

        Board b = gm.getBoard();
        for (Tile *t : b.getTiles())
        {
            cout << t->getCode() << "\n";
        }
        
        // TODO: Initialize game, start game loop
        // gm.initGame();
        // gm.runGame();
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "[ERROR] Unknown exception occurred!" << std::endl;
        return 1;
    }
}
