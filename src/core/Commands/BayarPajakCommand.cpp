#include "BayarPajakCommand.hpp"
#include "../Player/Player.hpp"
#include "../Bank/Bank.hpp"
#include <iostream>

BayarPajakCommand::BayarPajakCommand(Player* p, Bank& b, int amount, const std::string& name)
    : currentPlayer(p), bank(b), taxAmount(amount), taxName(name) {}

void BayarPajakCommand::execute() {
    if (currentPlayer == nullptr) {
        std::cerr << "Error: Pemain tidak valid saat memproses pembayaran pajak!" << std::endl;
        return;
    }

    std::cout << "\n=== PEMBAYARAN PAJAK ===" << std::endl;
    std::cout << "Pemain   : " << currentPlayer->getName() << std::endl;
    std::cout << "Jenis    : " << taxName << std::endl;
    std::cout << "Tagihan  : " << taxAmount << std::endl;
    std::cout << "Uang Awal: " << currentPlayer->getMoney() << std::endl;

    // 1. Cek likuiditas pemain (apakah uang cash mencukupi)
    if (currentPlayer->getMoney() >= taxAmount) {
        // 2. Jika cukup, uang ditarik dari pemain dan masuk ke bank
        bank.receiveFromPlayer(currentPlayer, taxAmount);
        
        std::cout << "\n[SUKSES] " << currentPlayer->getName() 
                  << " telah membayar " << taxName << "!" << std::endl;
        std::cout << "Sisa Uang: " << currentPlayer->getMoney() << std::endl;
    } else {
        // 3. Jika uang cash tidak mencukupi
        std::cout << "\n[PERINGATAN] " << currentPlayer->getName() 
                  << " tidak memiliki cukup uang tunai untuk membayar pajak!" << std::endl;
        
        /* * TUGAS LANJUTAN UNTUK M2:
         * Sesuai aturan Monopoli, jika uang tunai kurang, pemain HARUS mencari dana 
         * dengan cara menggadai (mortgage) properti atau menjual bangunan (rumah/hotel).
         * * Di sini, Anda sebaiknya TIDAK langsung membuat pemain bangkrut.
         * GameMaster harus menangkap state "Hutang" ini, lalu memberikan kesempatan 
         * kepada pemain untuk menjalankan aksi JUAL BANGUNAN atau GADAI PROPERTI.
         * Jika setelah aset dilikuidasi total hartanya tetap kurang dari taxAmount,
         * barulah GameMaster memanggil Bank::confesticatePlayerAssets() untuk menyatakan bangkrut.
         */
    }
    
    std::cout << "========================\n" << std::endl;
}