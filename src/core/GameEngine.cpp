#include "core/GameEngine.hpp"

#include <stdexcept>

GameEngine::GameEngine() : state() {
    loadGameConfig();
}

void GameEngine::startNewGame(const std::vector<std::string>& playerNames) {
    if (playerNames.empty()) {
        throw std::invalid_argument("Minimal harus ada satu pemain.");
    }

    for (const std::string& name : playerNames) {
        state.addPlayer(Player(name, state.getStartingCash()));
    }

    turnManager.startTurn(state.getCurrentPlayer(), state);
}

void GameEngine::loadGameConfig() {
    //Berikan opsi untuk menggunakan config default
    bool useDefault = CommandHandler::promptYesNo("Apakah anda ingin menggunakan konfigurasi default?");

    //Load property
    std::string propertyPath;
    if (useDefault){
        propertyPath = ConfigLoader::configPath + ConfigLoader::defaultPropertyFileName;
    }
    else{
        bool isValid = false;
        while (true){
            propertyPath = CommandHandler::promptInput("Masukkan lokasi path untuk konfigurasi property");
            try{
                auto property = ConfigLoader::loadProperty(propertyPath);
                //TODO do something
            }
            catch (GameException e){
                std::cout << e.what(); //TODO: tampilkan dengan renderer
            }
        }
    }

    // Load station
    std::string stationPath;
    if (useDefault){
        stationPath = ConfigLoader::configPath + ConfigLoader::defaultStationFileName;
    }
    else{
        while (true){
            stationPath = CommandHandler::promptInput("Masukkan lokasi path untuk konfigurasi station");
            try{
                auto station = ConfigLoader::loadRailroad(stationPath);
                StationPlot::setRentPriceTable(station);
                break;
            }
            catch (const GameException& e){
                std::cout << e.what(); //TODO: tampilkan dengan renderer
            }
        }
    }

    // Load utility
    std::string utilityPath;
    if (useDefault){
        utilityPath = ConfigLoader::configPath + ConfigLoader::defaultUtilityFileName;
    }
    else{
        while (true){
            utilityPath = CommandHandler::promptInput("Masukkan lokasi path untuk konfigurasi utility");
            try{
                auto utility = ConfigLoader::loadUtility(utilityPath);
                UtilityPlot::setRentPriceTable(utility);
                break;
            }
            catch (const GameException& e){
                std::cout << e.what(); //TODO: tampilkan dengan renderer
            }
        }
    }

    // Load action
    std::string actionPath;
    if (useDefault){
        actionPath = ConfigLoader::configPath + ConfigLoader::defaultActionFileName;
    }
    else{
        while (true){
            actionPath = CommandHandler::promptInput("Masukkan lokasi path untuk konfigurasi action");
            try{
                auto action = ConfigLoader::loadAction(actionPath);
                // TODO do something
                break;
            }
            catch (const GameException& e){
                std::cout << e.what(); //TODO: tampilkan dengan renderer
            }
        }
    }

    // Load tax
    std::string taxPath;
    if (useDefault){
        taxPath = ConfigLoader::configPath + ConfigLoader::defaultTaxFileName;
    }
    else{
        while (true){
            taxPath = CommandHandler::promptInput("Masukkan lokasi path untuk konfigurasi tax");
            try{
                auto tax = ConfigLoader::loadTax(taxPath);
                TaxPlot::setFlatAmount(std::get<0>(tax));
                TaxPlot::setPPH(std::get<1>(tax));
                TaxPlot::setPBM(std::get<2>(tax));
                break;
            }
            catch (const GameException& e){
                std::cout << e.what(); //TODO: tampilkan dengan renderer
            }
        }
    }

    // Load special
    std::string specialPath;
    if (useDefault){
        specialPath = ConfigLoader::configPath + ConfigLoader::defaultSpecialFileName;
    }
    else{
        while (true){
            specialPath = CommandHandler::promptInput("Masukkan lokasi path untuk konfigurasi special");
            try{
                auto special = ConfigLoader::loadSpecial(specialPath);
                state.setSalary(std::get<0>(special));
                state.setJailFine(std::get<1>(special));
                break;
            }
            catch (const GameException& e){
                std::cout << e.what(); //TODO: tampilkan dengan renderer
            }
        }
    }

    // Load misc
    std::string miscPath;
    if (useDefault){
        miscPath = ConfigLoader::configPath + ConfigLoader::defaultMiscFileName;
    }
    else{
        while (true){
            miscPath = CommandHandler::promptInput("Masukkan lokasi path untuk konfigurasi misc");
            try{
                auto misc = ConfigLoader::loadMisc(miscPath);
                state.setMaxTurn(std::get<0>(misc));
                state.setSalary(std::get<0>(misc));
                break;
            }
            catch (const GameException& e){
                std::cout << e.what(); //TODO: tampilkan dengan renderer
            }
        }
    }
}


void GameEngine::loadGame(const GameState& loadedState) {
    state = loadedState;
}

void GameEngine::run(const std::vector<std::string>& scriptedCommands) {
    for (const std::string& rawCommand : scriptedCommands) {
        if (state.isGameOver()) {
            break;
        }

        const auto command = commandHandler.parse(rawCommand);
        commandHandler.execute(*command, state, effectResolver, turnManager);

        if (state.hasSingleActivePlayer()) {
            state.setGameOver(true);
        }
    }
}

bool GameEngine::isGameOver() const {
    return state.isGameOver();
}

std::vector<Player> GameEngine::getWinner() const {
    std::vector<Player> winners;
    const auto& players = state.getPlayers();
    if (players.empty()) {
        return winners;
    }

    const auto activePlayers = state.getActivePlayers();
    if (activePlayers.empty()) {
        return winners;
    }
    if (activePlayers.size() == 1) {
        winners.push_back(activePlayers.front());
        return winners;
    }

    int highestCash = -1;
    for (const Player& player : activePlayers) {
        if (player.getCash() > highestCash) {
            highestCash = player.getCash();
        }
    }

    for (const Player& player : activePlayers) {
        if (player.getCash() == highestCash) {
            winners.push_back(player);
        }
    }

    return winners;
}

GameState& GameEngine::getState() {
    return state;
}

const GameState& GameEngine::getState() const {
    return state;
}
