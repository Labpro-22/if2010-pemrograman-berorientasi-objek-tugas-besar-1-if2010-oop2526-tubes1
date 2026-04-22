#include "../../include/views/CommandProcessor.hpp"
#include "../../include/views/BoardView.hpp"
#include "../../include/views/PropertyView.hpp"
#include "../../include/core/GameController.hpp"

#include <iostream>
#include <sstream>

using namespace std;

CommandProcessor::CommandProcessor(GameController* gc, BoardView* bv, PropertyView* pv)
    : gameController(gc), boardView(bv), propertyView(pv) {}

void CommandProcessor::readCommand() {
    string input;
    cout << "> ";
    getline(cin, input);

    executeCommand(input);
}

void CommandProcessor::executeCommand(const string& cmd) {
    stringstream ss(cmd);
    string token;
    ss >> token;

    if (token == "CETAK_PAPAN") {
        if (gameController != nullptr && boardView != nullptr) {
            boardView->showBoard(gameController->getBoard(), gameController->getPlayers());
        } else {
            cout << "Error: GameController atau BoardView tidak diinisialisasi." << endl;
        }
    }
    else if (token == "CETAK_PROPERTI") {
        cout << "[INFO] Command CETAK_PROPERTI belum diimplementasi" << endl;
    }
    else if (token == "EXIT") {
        cout << "[INFO] Game ditutup." << endl;
        exit(0);
    }
    else if (token == "HELP") {
        cout << "Commands tersedia:\n";
        cout << "  CETAK_PAPAN    - Menampilkan board game\n";
        cout << "  CETAK_PROPERTI - Menampilkan detail properti\n";
        cout << "  EXIT           - Keluar dari game\n";
        cout << "  HELP           - Tampilkan help ini\n";
    }
    else {
        cout << "Perintah '" << cmd << "' tidak dikenali. Ketik HELP untuk bantuan." << endl;
    }
}
