#include "AturDaduCommand.hpp"
#include "../Dice/Dice.hpp"
#include <iostream>

// --- Implementasi InvalidDiceValueException ---
InvalidDiceValueException::InvalidDiceValueException(int v1, int v2) {
    errorMessage = "Exception: Nilai dadu tidak valid (" + std::to_string(v1) + ", " + 
                   std::to_string(v2) + ")! Kedua dadu harus bernilai antara 1 hingga 6.";
}

const char* InvalidDiceValueException::what() const noexcept {
    return errorMessage.c_str();
}

// --- Implementasi AturDaduCommand ---
AturDaduCommand::AturDaduCommand(Dice& d, int v1, int v2) 
    : dice(d), targetVal1(v1), targetVal2(v2) {}

void AturDaduCommand::execute() {
    // 1. Validasi rentang nilai dadu
    if (targetVal1 < 1 || targetVal1 > 6 || targetVal2 < 1 || targetVal2 > 6) {
        // Melempar exception jika nilai di luar batas 1-6
        throw InvalidDiceValueException(targetVal1, targetVal2);
    }

    // 2. Jika valid, atur dadu secara manual
    dice.setManual(targetVal1, targetVal2);

    // 3. Konfirmasi output ke terminal
    std::cout << "Dadu berhasil diatur secara manual!" << std::endl;
    std::cout << "Dadu 1 = " << dice.getDaduVal1() << ", Dadu 2 = " << dice.getDaduVal2() 
              << " (Total: " << dice.getTotal() << ")" << std::endl;
    
    // Catatan: Jika ini murni untuk pengaturan state sebelum LemparDaduCommand, 
    // Anda tidak perlu menggerakkan player di sini. 
    // Pergerakan player tetap dieksekusi saat command melempar dadu atau command jalan dipanggil.
}