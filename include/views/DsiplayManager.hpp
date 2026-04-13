#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include <iostream>
#include "views/BoardRenderer.hpp"
#include "views/PropertyRenderer.hpp"
#include "views/TransactionLogger.hpp"

class DisplayManager {
private:
    // Sesuai diagram: menggunakan referensi/objek renderer
    BoardRenderer& boardRenderer;
    PropertyRenderer& propRenderer;

public:
    DisplayManager(BoardRenderer& br, PropertyRenderer& pr) 
        : boardRenderer(br), propRenderer(pr) {}

    // Method utama untuk interaksi game
    void printBoard(GameMaster& gm);
    void printPropertyDeed(Property& prop);
    void printPlayerProperties(Player& player);
    
    // Kamu yang pegang ini!
    void printTransactionLog(TransactionLogger& logger, int n, std::ostream& out = std::cout);
    
    void printWinner(GameMaster& gm);
    void printMessage(std::string msg); // Typo di diagram 'prinMassage' kita perbaiki ya
};

#endif