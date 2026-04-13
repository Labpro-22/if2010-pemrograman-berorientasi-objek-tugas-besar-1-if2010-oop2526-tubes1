#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class InputHandler {
public:
    // Mengambil satu baris input dan memecahnya jadi kata-kata (tokens)
    // Contoh: "MUAT save_game.txt" -> ["MUAT", "save_game.txt"]
    static std::vector<std::string> getCommand();

    // Meminta input angka dengan range tertentu (untuk menu)
    static int getIntInput(int min, int max);

    // Konfirmasi Yes/No
    static bool confirm(std::string message);

private:
    // Helper untuk mengubah string jadi uppercase biar ga sensitif (BUy == buy)
    static std::string toUpper(std::string str);
};

#endif