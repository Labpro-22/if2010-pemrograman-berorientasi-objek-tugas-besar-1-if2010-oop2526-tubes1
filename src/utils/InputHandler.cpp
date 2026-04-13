#include "utils/InputHandler.hpp"
#include <sstream>
#include <limits>

std::string InputHandler::toUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::vector<std::string> InputHandler::getCommand() {
    std::string line;
    std::cout << "\n> ";
    std::getline(std::cin >> std::ws, line);

    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string word;
    
    while (ss >> word) {
        tokens.push_back(toUpper(word));
    }
    return tokens;
}

int InputHandler::getIntInput(int min, int max) {
    int input;
    while (true) {
        std::cout << "Pilih (" << min << "-" << max << "): ";
        if (std::cin >> input && input >= min && input <= max) {
            return input;
        } else {
            std::cout << "Input tidak valid! Masukkan angka antara " << min << " dan " << max << ".\n";
            std::cin.clear(); // Hapus flag error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang buffer sisa
        }
    }
}

bool InputHandler::confirm(std::string message) {
    std::string choice;
    while (true) {
        std::cout << message << " (Y/N): ";
        std::cin >> choice;
        choice = toUpper(choice);
        if (choice == "Y" || choice == "YES") return true;
        if (choice == "N" || choice == "NO") return false;
    }
}