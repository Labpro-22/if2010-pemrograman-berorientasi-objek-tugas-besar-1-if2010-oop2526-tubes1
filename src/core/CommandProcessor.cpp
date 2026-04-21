#include "../../include/core/CommandProcessor.hpp"
#include "../../include/views/BoardView.hpp"
#include "../../include/views/PropertyView.hpp"
// #include "../../include/views/GameView.hpp"
// #include "../../include/core/GameController.hpp"

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

        // sementara dummy (belum integrasi penuh)
        // boardView->showBoard(...);
    }

    else if (token == "CETAK_PROPERTI") {
        // sementara dummy (belum integrasi penuh)
        // Property* prop = gameController->getPropertyByName(propName);
        // propertyView->showPropertyDetail(prop);
    }

    else if (token == "EXIT") {
        exit(0);
    }

    else {
        cout << "Perintah " << cmd << " tidak dikenali."<< endl;
    }
}