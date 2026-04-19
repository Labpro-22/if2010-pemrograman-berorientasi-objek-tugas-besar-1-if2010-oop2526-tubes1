#ifndef LELANGCOMMAND_HPP
#define LELANGCOMMAND_HPP

#include "Command.hpp"
#include <vector>

// Forward declarations
class AuctionManager;
class GameMaster;
class Property;
class Player;
class Bank;

class LelangCommand : public Command {
private:
    AuctionManager& auctionManager;
    GameMaster& gameMaster;
    Bank& bank;
    Property* targetProperty;

public:
    // Konstruktor menerima referensi ke manager dan properti yang akan dilelang
    LelangCommand(AuctionManager& am, GameMaster& gm, Bank& b, Property* prop);

    // Menjalankan alur lelang dari awal hingga selesai
    void execute() override;
};

#endif