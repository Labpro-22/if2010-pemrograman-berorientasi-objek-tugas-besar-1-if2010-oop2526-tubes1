#ifndef BELICOMMAND_HPP
#define BELICOMMAND_HPP

#include "Command.hpp" 

// Forward declarations untuk menghindari circular dependency dan mempercepat kompilasi
class Player;
class Property;
class Bank;

class BeliCommand : public Command {
private:
    Player* currentPlayer;
    Property* targetProperty;
    Bank& bank; // Referensi ke Bank utama

public:
    // Konstruktor menerima pemain yang sedang aktif, properti yang diinjak, dan referensi bank
    BeliCommand(Player* p, Property* prop, Bank& b);

    // Override metode execute
    void execute() override;
};

#endif