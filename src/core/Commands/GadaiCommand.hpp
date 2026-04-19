#ifndef GADAICOMMAND_HPP
#define GADAICOMMAND_HPP

#include "Command.hpp" // Asumsi ada base interface Command

// Forward declarations
class Player;
class Property;
class Bank;

class GadaiCommand : public Command {
private:
    Player* currentPlayer;
    Property* targetProperty;
    Bank& bank;

public:
    // Konstruktor menerima pemain, properti yang akan digadai, dan referensi bank
    GadaiCommand(Player* p, Property* prop, Bank& b);

    // Override metode execute dari kelas induk
    void execute() override;
};

#endif