#ifndef TEBUSCOMMAND_HPP
#define TEBUSCOMMAND_HPP

#include "Command.hpp" 

// Forward declarations
class Player;
class Property;
class Bank;

class TebusCommand : public Command {
private:
    Player* currentPlayer;
    Property* targetProperty;
    Bank& bank;

public:
    // Konstruktor menerima pemain yang melakukan tebus, properti yang ditebus, dan bank
    TebusCommand(Player* p, Property* prop, Bank& b);

    // Override metode execute dari kelas induk Command
    void execute() override;
};

#endif