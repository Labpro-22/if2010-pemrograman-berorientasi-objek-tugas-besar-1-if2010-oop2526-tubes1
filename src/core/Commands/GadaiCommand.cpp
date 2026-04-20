#include "GadaiCommand.hpp"
#include "../Player/Player.hpp"
#include "../Property/Property.hpp"
#include "../Bank/Bank.hpp"
#include <iostream>

GadaiCommand::GadaiCommand(Player* p, Property* prop, Bank& b)
    : currentPlayer(p), targetProperty(prop), bank(b) {}

void GadaiCommand::execute() {
    if (currentPlayer == nullptr || targetProperty == nullptr) {
        std::cerr << "Error: Entitas pemain atau properti tidak valid untuk transaksi gadai!" << std::endl;
        return;
    }

    std::cout << "\n=== PROSES GADAI PROPERTI ===" << std::endl;
    std::cout << "Pemain   : " << currentPlayer->getName() << std::endl;
    std::cout << "Properti : " << targetProperty->getName() << std::endl;

    // 1. Validasi Kepemilikan: Hanya pemilik yang bisa menggadaikan
    if (targetProperty->getOwner() != currentPlayer) {
        std::cout << "[GAGAL] Anda tidak bisa menggadaikan properti yang bukan milik Anda!" << std::endl;
        return;
    }

    // 2. Validasi Status Gadai: Tidak bisa menggadai properti yang sudah tergadai
    if (targetProperty->isMortgaged()) {
        std::cout << "[GAGAL] Properti " << targetProperty->getName() 
                  << " sudah dalam status tergadai!" << std::endl;
        return;
    }

    // 3. Validasi Bangunan: Aturan Monopoli melarang menggadai tanah yang masih memiliki bangunan
    // Asumsi: Method hasBuildings() ada di kelas Property atau turunannya (Street)
    // Jika game Anda mengizinkan gadai dengan bangunan (atau otomatis dijual), bagian ini bisa disesuaikan.
    if (targetProperty->hasBuildings()) {
        std::cout << "[GAGAL] Anda harus menjual semua rumah/hotel di properti ini "
                  << "dan di satu kompleks warna yang sama sebelum bisa menggadaikan tanahnya!" << std::endl;
        return;
    }

    // 4. Proses Transaksi
    // Bank memberikan uang gadai kepada pemain
    int mortgagePrice = targetProperty->getMortgagePrice();
    bank.payPlayer(currentPlayer, mortgagePrice);

    // Ubah status properti menjadi tergadai
    targetProperty->setMortgaged(true);

    std::cout << "\n[SUKSES] " << currentPlayer->getName() << " berhasil menggadaikan " 
              << targetProperty->getName() << "!" << std::endl;
    std::cout << "Dana yang diterima : " << mortgagePrice << std::endl;
    std::cout << "Total Uang Anda  : " << currentPlayer->getMoney() << std::endl;
    std::cout << "=============================\n" << std::endl;
}