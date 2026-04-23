#include "core/GameEngine.hpp"

#include "core/GameException.hpp"

GameEngine::GameEngine() : state() {
    loadGameConfig();
}

void GameEngine::startNewGame(const std::vector<std::string>& playerNames) {
    if (playerNames.empty()) {
        throw InvalidInputException("Minimal harus ada satu pemain.");
    }

    for (const std::string& name : playerNames) {
        state.addPlayer(Player(name, state.getStartingCash()));
    }

    turnManager.startTurn(state.getCurrentPlayer(), state);
}

//Lambda function agar loadGameConfig tidak redundan
template <typename Func>
auto load(Func loader, std::string defaultFileName, std::string type, bool useDefault){
    if (useDefault){
        return loader(ConfigLoader::configPath + defaultFileName);
    }
    std::string prompt = "Masukkan nama file untuk konfigurasi " + type + " (file harus disimpan di folder config)";
    while (true){
        std::string fileName = CommandHandler::promptInput(prompt);
        try{
            return loader(ConfigLoader::configPath + fileName);
        }
        catch (const GameException& e){
            std::cout << e.what();
        }
    }
}

void GameEngine::loadGameConfig() {
    //Berikan opsi untuk menggunakan config default
    bool useDefault = CommandHandler::promptYesNo("Apakah anda ingin menggunakan konfigurasi default?");

    //Load property
    auto property = load(ConfigLoader::loadProperty, ConfigLoader::defaultPropertyFileName,
        "properti", useDefault);

    //Load station
    auto station = load(ConfigLoader::loadRailroad, ConfigLoader::defaultStationFileName,
        "railroad", useDefault);
    
    StationPlot::setRentPriceTable(station);

    //Load utility
    auto utility = load(ConfigLoader::loadUtility, ConfigLoader::defaultUtilityFileName,
        "utilitas", useDefault);
    UtilityPlot::setRentPriceTable(utility);

    //Load action
    auto action = load(ConfigLoader::loadProperty, ConfigLoader::defaultPropertyFileName,
        "petak aksi", useDefault);
    std::string actionPath;
    std::vector<std::pair<int, std::unique_ptr<Plot>>> action;
    if (useDefault){
        actionPath = ConfigLoader::configPath + ConfigLoader::defaultActionFileName;
    }

    // Load tax
    auto tax = load(ConfigLoader::loadTax, ConfigLoader::defaultTaxFileName,
        "pajak", useDefault);
    std::string taxPath;

    TaxPlot::setFlatAmount(std::get<0>(tax));
    TaxPlot::setPPH(std::get<1>(tax));
    TaxPlot::setPBM(std::get<2>(tax));

    // Load special
    auto special = load(ConfigLoader::loadSpecial, ConfigLoader::defaultSpecialFileName,
        "spesial", useDefault);
    std::string specialPath;
    if (useDefault){
        specialPath = ConfigLoader::configPath + ConfigLoader::defaultSpecialFileName;
    }

    state.setSalary(std::get<0>(special));
    state.setJailFine(std::get<1>(special));

    // Load misc
    auto misc = load(ConfigLoader::loadMisc, ConfigLoader::defaultMiscFileName,
        "lainnya", useDefault);
   
    state.setMaxTurn(std::get<0>(misc));
    state.setSalary(std::get<1>(misc));

    //Buat board dari konfigurasi properti dan petak aksi
    std::vector<std::unique_ptr<Plot>> tiles;
    int i = 0, j = 0;
    while (i < property.size() && j < action.size()){
        if (property[i].first < action[j].first) {
            tiles.push_back(std::move(property[i].second));
            i++;
        }
        else{
            tiles.push_back(std::move(action[j].second));
            j++;
        }
    }

    while (i < property.size()){
        tiles.push_back(std::move(property[i].second));
        i++;
    }

    while (j < action.size()){
        tiles.push_back(std::move(action[j].second));
        j++;
    }
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
