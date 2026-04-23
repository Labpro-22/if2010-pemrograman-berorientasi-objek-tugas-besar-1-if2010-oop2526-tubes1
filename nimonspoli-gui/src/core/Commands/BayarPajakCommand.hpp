#pragma once
#include "../Bank/Bank.hpp"
#include "../Board/Board.hpp"
#include "../utils/TransactionLogger.hpp"
#include "../Exceptions/FinancialException.hpp"
#include "./Command.hpp"
#include "../Player/Player.hpp"
#include "../utils/ConfigLoader.hpp" 

class BayarPajakCommand : public Command
{
private:
    Player *p;
    TaxTile *tile;
    Bank *bank;
    TransactionLogger *logger;
    TaxConfig taxConfig;
    int turnNumber;

public:
    BayarPajakCommand(Player *p, TaxTile *tile, Bank *bank,
                      TransactionLogger *logger, TaxConfig taxConfig, int turn);
    void execute(GameMaster& gm) override;

private:
    void handlePPH();
    void handlePBM();
};