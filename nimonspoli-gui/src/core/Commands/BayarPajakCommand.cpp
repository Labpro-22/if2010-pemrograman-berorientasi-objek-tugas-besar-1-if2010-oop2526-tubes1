#include "BayarPajakCommand.hpp"
#include <iostream>

BayarPajakCommand::BayarPajakCommand(Player *p, TaxTile *tile, Bank *bank,
                                     TransactionLogger *logger, TaxConfig taxConfig, int turn)
    : p(p), tile(tile), bank(bank), logger(logger), taxConfig(taxConfig), turnNumber(turn) {}

void BayarPajakCommand::execute(GameMaster&)
{
    if (tile->getCode() == "PPH")
    {
        handlePPH();
    }
    else if (tile->getCode() == "PBM")
    {
        handlePBM();
    }
}

void BayarPajakCommand::handlePPH()
{
    int flatAmount = static_cast<int>(taxConfig.pphFlat);
    int pctAmount = static_cast<int>(taxConfig.pphPercentage); 

    std::cout << "\nKamu mendarat di Pajak Penghasilan (PPH)!\n";
    std::cout << "Pilih opsi pembayaran pajak:\n";
    std::cout << "  1. Bayar flat M" << flatAmount << "\n";
    std::cout << "  2. Bayar " << pctAmount << "% dari total kekayaan\n";
    std::cout << "(Pilih sebelum menghitung kekayaan!)\n";
    std::cout << "Pilihan (1/2): ";

    int choice;
    std::cin >> choice;

    if (choice == 1)
    {
        if (!p->canAfford(flatAmount))
        {
            std::cout << "Kamu tidak mampu membayar pajak flat M" << flatAmount << "!\n";
            std::cout << "Uang kamu saat ini: M" << p->getBalance() << "\n";
            throw InsufficientFundsException(p->getUsername(), "Pajak Penghasilan (flat)");
        }
        int before = p->getBalance();
        *p -= flatAmount;
        std::cout << "Uang kamu: M" << before << " -> M" << p->getBalance() << "\n";
        logger->addLog(turnNumber, p->getUsername(), "PAJAK",
                       "Bayar PPH flat M" + std::to_string(flatAmount));
    }
    else
    {
        
        int wealth = p->getWealth();
        int taxAmt = (wealth * pctAmount) / 100;

        std::cout << "\nTotal kekayaan kamu:\n";
        std::cout << "  - Uang tunai       : M" << p->getBalance() << "\n";
        std::cout << "  - Nilai properti   : M" << (wealth - p->getBalance()) << "\n";
        std::cout << "  Total              : M" << wealth << "\n";
        std::cout << "Pajak " << pctAmount << "%          : M" << taxAmt << "\n";

        if (!p->canAfford(taxAmt))
        {
            std::cout << "Kamu tidak mampu membayar pajak M" << taxAmt << "!\n";
            std::cout << "Uang kamu saat ini: M" << p->getBalance() << "\n";
            throw InsufficientFundsException(p->getUsername(), "Pajak Penghasilan (%)");
        }
        int before = p->getBalance();
        *p -= taxAmt;
        std::cout << "Uang kamu: M" << before << " -> M" << p->getBalance() << "\n";
        logger->addLog(turnNumber, p->getUsername(), "PAJAK",
                       "Bayar PPH " + std::to_string(pctAmount) +
                           "% = M" + std::to_string(taxAmt));
    }
}

void BayarPajakCommand::handlePBM()
{
    int flatAmount = static_cast<int>(taxConfig.pbmFlat);

    std::cout << "\nKamu mendarat di Pajak Barang Mewah (PBM)!\n";
    std::cout << "Pajak sebesar M" << flatAmount << " langsung dipotong.\n";

    if (!p->canAfford(flatAmount))
    {
        std::cout << "Kamu tidak mampu membayar pajak!\n";
        std::cout << "Uang kamu saat ini: M" << p->getBalance() << "\n";
        throw InsufficientFundsException(p->getUsername(), "Pajak Barang Mewah");
    }
    int before = p->getBalance();
    *p -= flatAmount;
    std::cout << "Uang kamu: M" << before << " -> M" << p->getBalance() << "\n";
    logger->addLog(turnNumber, p->getUsername(), "PAJAK",
                   "Bayar PBM M" + std::to_string(flatAmount));
}