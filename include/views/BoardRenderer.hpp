#ifndef BOARD_RENDERER_HPP
#define BOARD_RENDERER_HPP

#include <string>
#include <vector>
#include <iostream>

class BoardRenderer {
public:
    // ANSI Color Codes
    const std::string RESET = "\033[0m";
    const std::string BOLD  = "\033[1m";
    const std::string RED   = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW= "\033[33m";
    const std::string CYAN  = "\033[36m";
    const std::string MAGENTA= "\033[35m";

    // Fungsi utama
    void renderBoard(const std::vector<std::string>& tileCodes, int currTurn);
    
private:
    // Helper untuk menggambar garis horizontal
    void printBorder(int cells);
};

#endif