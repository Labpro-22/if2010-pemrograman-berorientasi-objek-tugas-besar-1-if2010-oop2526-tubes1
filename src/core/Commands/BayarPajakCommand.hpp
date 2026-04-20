#ifndef BAYARPAJAKCOMMAND_HPP
#define BAYARPAJAKCOMMAND_HPP

#include "Command.hpp" // Asumsi ada interface base Command
#include <string>

// Forward declarations untuk mencegah circular dependency
class Player;
class Bank;

class BayarPajakCommand : public Command {
private:
    Player* currentPlayer;
    Bank& bank;
    int taxAmount;
    std::string taxName;

public:
    // Konstruktor menerima pemain, referensi bank, nominal pajak, dan nama pajak (opsional)
    BayarPajakCommand(Player* p, Bank& b, int amount, const std::string& name = "Pajak");

    // Override metode execute dari kelas induk Command
    void execute() override;
};

#endif