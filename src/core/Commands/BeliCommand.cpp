#include "BeliCommand.hpp"
#include "../Player/Player.hpp"
#include "../Property/Property.hpp"
#include "../Bank/Bank.hpp"
#include <iostream>

BeliCommand::BeliCommand(Player* p, Property* prop, Bank& b)
    : currentPlayer(p), targetProperty(prop), bank(b) {}

void BeliCommand::execute() {
    if (currentPlayer == nullptr || targetProperty == nullptr) {
        std::cerr << "Error: Pemain atau properti tidak valid untuk transaksi pembelian!" << std::endl;
        return;
    }

    std::cout << "\n=== PROSES PEMBELIAN PROPERTI ===" << std::endl;
    std::cout << "Pemain   : " << currentPlayer->getName() << std::endl;
    std::cout << "Properti : " << targetProperty->getName() << std::endl;

    // 1. Cek apakah properti masih kosong (belum ada pemiliknya)
    if (targetProperty->getOwner() != nullptr) {
        std::cout << "Transaksi Batal: Properti " << targetProperty->getName() 
                  << " sudah dimiliki oleh " << targetProperty->getOwner()->getName() << "!" << std::endl;
        return;
    }

    // 2. Dapatkan harga dasar properti
    int price = targetProperty->getLandPrice();
    std::cout << "Harga Tanah : " << price << std::endl;
    std::cout << "Uang Pemain : " << currentPlayer->getMoney() << std::endl;

    // 3. Validasi likuiditas (apakah uang pemain mencukupi)
    if (currentPlayer->getMoney() >= price) {
        // Pemain membayar ke bank (mengurangi uang pemain)
        bank.receiveFromPlayer(currentPlayer, price);

        // Atur kepemilikan di objek properti
        targetProperty->setOwner(currentPlayer);

        // Tambahkan properti ke dalam daftar aset yang dimiliki pemain
        currentPlayer->addProperty(targetProperty);

        std::cout << "\n[SUKSES] " << currentPlayer->getName() << " berhasil membeli " 
                  << targetProperty->getName() << "!" << std::endl;
        std::cout << "Sisa uang " << currentPlayer->getName() << ": " << currentPlayer->getMoney() << std::endl;
    } else {
        // Jika uang tidak cukup
        std::cout << "\n[GAGAL] " << currentPlayer->getName() 
                  << " tidak memiliki cukup uang untuk membeli " << targetProperty->getName() << "!" << std::endl;
        
        /* * CATATAN INTEGRASI: 
         * Sesuai aturan Monopoli, jika pemain tidak bisa (atau tidak mau) membeli properti 
         * yang diinjaknya, properti tersebut WAJIB dilelang.
         * * Di dalam game loop atau GameMaster Anda, pastikan untuk mengecek hasil dari BeliCommand.
         * Jika pembelian gagal atau dilewati (pass), GameMaster harus langsung membuat 
         * dan mengeksekusi objek LelangCommand yang sudah kita buat sebelumnya.
         */
    }
    std::cout << "=================================\n" << std::endl;
}