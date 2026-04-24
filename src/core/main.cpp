#include "core/GameEngine.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "core/GameException.hpp"
#include "core/GameRenderer.hpp"

namespace {
std::string trim(std::string text) {
    const std::size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

int readPositiveInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            throw InvalidInputException("Input dihentikan sebelum permainan dimulai.");
        }

        input = trim(input);
        try {
            std::size_t parsedCount = 0;
            const int value = std::stoi(input, &parsedCount);
            if (parsedCount == input.size() && value > 0) {
                return value;
            }
        } catch (const std::exception&) {
        }

        std::cout << "Masukkan bilangan bulat positif.\n";
    }
}

std::vector<std::string> readPlayerNames(int count) {
    std::vector<std::string> names;
    names.reserve(static_cast<std::size_t>(count));

    for (int i = 0; i < count; ++i) {
        while (true) {
            std::cout << "Nama pemain " << (i + 1) << ": ";
            std::string name;
            if (!std::getline(std::cin, name)) {
                throw InvalidInputException("Input nama pemain dihentikan.");
            }

            name = trim(name);
            if (name.empty()) {
                std::cout << "Nama pemain tidak boleh kosong.\n";
                continue;
            }
            if (std::find(names.begin(), names.end(), name) != names.end()) {
                std::cout << "Nama pemain harus unik.\n";
                continue;
            }

            names.push_back(name);
            break;
        }
    }
    return names;
}
}  // namespace

int main() {
    try {
        GameEngine engine;

        std::cout << "=== NIMONSPOLI ===\n";
        const int playerCount = readPositiveInt("Jumlah pemain: ");
        const std::vector<std::string> playerNames = readPlayerNames(playerCount);
        engine.startNewGame(playerNames);

        std::cout << "Ketik EXIT untuk keluar.\n";
        while (!engine.isGameOver()) {
            const GameState& state = engine.getState();
            GameRenderer::showBoard(state);
            std::cout << "Giliran " << state.getCurrentTurn() << "/" << state.getMaxTurn()
                      << " - " << state.getCurrentPlayer().getUsername() << "\n";

            std::cout << "Perintah: ";
            std::string command;
            if (!std::getline(std::cin, command)) {
                std::cout << "\nInput dihentikan.\n";
                break;
            }

            command = trim(command);
            if (command.empty()) {
                continue;
            }
            if (command == "EXIT") {
                break;
            }

            try {
                engine.run({command});
                const auto logs = engine.getState().getRecentLogs(1);
                if (!logs.empty()) {
                    GameRenderer::showLogger(logs.back());
                }
            } catch (const GameException& e) {
                std::cout << e.what() << "\n";
            }
        }

        const std::vector<Player> winners = engine.getWinner();
        if (winners.empty()) {
            std::cout << "Permainan selesai tanpa pemenang.\n";
            return 0;
        }

        if (winners.size() == 1) {
            std::cout << "Pemenang: " << winners.front().getUsername() << "\n";
            return 0;
        }

        std::cout << "Permainan seri antara: ";
        for (std::size_t i = 0; i < winners.size(); ++i) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << winners[i].getUsername();
        }
        std::cout << "\n";
        return 0;
    } catch (const GameException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error tak terduga: " << e.what() << "\n";
        return 1;
    }
}
