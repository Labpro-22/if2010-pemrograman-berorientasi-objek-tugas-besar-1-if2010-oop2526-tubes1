#include "utils/InputHandler.hpp"
#include <iostream>

int main() {
    std::cout << "--- TESTING INPUT HANDLER ---\n";

    // Test 1: Command Parsing
    std::cout << "Coba ketik perintah (misal: MUAT sesi1):";
    std::vector<std::string> cmd = InputHandler::getCommand();
    if (!cmd.empty()) {
        std::cout << "Command: " << cmd[0] << "\n";
        if (cmd.size() > 1) std::cout << "Argumen: " << cmd[1] << "\n";
    }

    // Test 2: Validasi Angka
    int menu = InputHandler::getIntInput(1, 3);
    std::cout << "Kamu pilih menu: " << menu << "\n";

    // Test 3: Konfirmasi
    if (InputHandler::confirm("Apakah kamu ingin menyimpan game?")) {
        std::cout << "Menyimpan...\n";
    } else {
        std::cout << "Batal simpan.\n";
    }

    return 0;
}