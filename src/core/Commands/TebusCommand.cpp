#include "TebusCommand.hpp"
#include "../Player/Player.hpp"
#include "../Property/Property.hpp"
#include "../Bank/Bank.hpp"
#include <iostream>

TebusCommand::TebusCommand(Player* p, Property* prop, Bank& b)
    : currentPlayer(p), targetProperty(prop), bank(b) {}

void TebusCommand::execute() {
    if (currentPlayer == nullptr || targetProperty == nullptr) {
        std::cerr << "Error: Pemain atau properti tidak valid untuk transaksi tebus!" << std::endl;
        return;
    }

    std::cout << "\n=== PROSES TEBUS PROPERTI ===" << std::endl;
    std::cout << "Pemain   : " << currentPlayer->getName() << std::endl;
    std::cout << "Properti : " << targetProperty->getName() << std::endl;

    // 1. Validasi Kepemilikan (Pemain hanya bisa menebus properti miliknya sendiri)
    if (targetProperty->getOwner() != currentPlayer) {
        std::cout << "[GAGAL] Anda tidak bisa menebus properti yang bukan milik Anda!" << std::endl;
        return;
    }

    // 2. Validasi Status Gadai (Apakah properti sedang digadaikan?)
    // Asumsi: kelas Property memiliki method isMortgaged() atau getStatus()
    if (!targetProperty->isMortgaged()) {
        std::cout << "[GAGAL] Properti " << targetProperty->getName() 
                  << " tidak sedang dalam status tergadai!" << std::endl;
        return;
    }

    // 3. Hitung Biaya Tebus
    // Asumsi: Property memiliki method getMortgagePrice() untuk harga gadainya.
    // Di Nimonspoli/Monopoli, biaya tebus biasanya: Harga Gadai + Bunga (misal 10%)
    int mortgagePrice = targetProperty->getMortgagePrice();
    int interest = mortgagePrice * 0.1; // Bunga 10%
    int tebusCost = mortgagePrice + interest;

    std::cout << "Biaya Tebus : " << tebusCost << " (Pokok: " << mortgagePrice 
              << " + Bunga: " << interest << ")" << std::endl;
    std::cout << "Uang Pemain : " << currentPlayer->getMoney() << std::endl;

    // 4. Validasi Likuiditas
    if (currentPlayer->getMoney() >= tebusCost) {
        // Pemain membayar biaya tebus ke Bank
        bank.receiveFromPlayer(currentPlayer, tebusCost);

        // Ubah status properti menjadi tidak tergadai
        // Asumsi: Property memiliki method setMortgaged(bool)
        targetProperty->setMortgaged(false);

        std::cout << "\n[SUKSES] " << currentPlayer->getName() << " berhasil menebus " 
                  << targetProperty->getName() << "!" << std::endl;
        std::cout << "Sisa uang : " << currentPlayer->getMoney() << std::endl;
    } else {
        std::cout << "\n[GAGAL] " << currentPlayer->getName() 
                  << " tidak memiliki cukup uang untuk menebus properti ini." << std::endl;
    }
    
    std::cout << "=============================\n" << std::endl;
}