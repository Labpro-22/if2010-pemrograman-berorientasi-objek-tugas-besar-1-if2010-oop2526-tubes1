#include "../../include/core/Game.hpp"
#include "../../include/core/TurnManager.hpp"
#include "../../include/core/TradeManager.hpp"
#include "../../include/data-layer/Config.hpp"
#include "../../include/data-layer/ConfigComposer.hpp"
#include "../../include/data-layer/GameStateLoader.hpp"
#include "../../include/data-layer/GameStateSaver.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/TradeToPlayer.hpp"
#include "../../include/utils/BoardBuilder.hpp"
#include "../../include/utils/PropertyTile.hpp"
#include "../../include/utils/Tile.hpp"

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

static int totalChecks = 0;
static int failedChecks = 0;

static const std::string CONFIG_DIR = "config/basic";
static const std::string SAVE_PATH = "data/test_turn_trade_game_state.txt";

static void printSeparator()
{
    std::cout << "------------------------------------------------------------\n";
}

static void check(bool condition, const std::string& message)
{
    totalChecks++;

    if (condition)
    {
        std::cout << "[PASS] " << message << '\n';
    }
    else
    {
        failedChecks++;
        std::cout << "[FAIL] " << message << '\n';
    }
}

static void expectInt(int actual, int expected, const std::string& message)
{
    totalChecks++;

    if (actual == expected)
    {
        std::cout << "[PASS] " << message << '\n';
    }
    else
    {
        failedChecks++;
        std::cout << "[FAIL] " << message
                  << " | expected: " << expected
                  << ", got: " << actual << '\n';
    }
}

static void expectSize(std::size_t actual, std::size_t expected, const std::string& message)
{
    totalChecks++;

    if (actual == expected)
    {
        std::cout << "[PASS] " << message << '\n';
    }
    else
    {
        failedChecks++;
        std::cout << "[FAIL] " << message
                  << " | expected: " << expected
                  << ", got: " << actual << '\n';
    }
}

static void expectString(const std::string& actual, const std::string& expected, const std::string& message)
{
    totalChecks++;

    if (actual == expected)
    {
        std::cout << "[PASS] " << message << '\n';
    }
    else
    {
        failedChecks++;
        std::cout << "[FAIL] " << message
                  << " | expected: " << expected
                  << ", got: " << actual << '\n';
    }
}

static void ensureDataDirectory()
{
#ifdef _WIN32
    std::system("if not exist data mkdir data");
#else
    std::system("mkdir -p data");
#endif
}

static std::string readWholeFile(const std::string& path)
{
    std::ifstream in(path);

    if (!in.is_open())
    {
        return "";
    }

    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

static bool containsText(const std::string& text, const std::string& pattern)
{
    return text.find(pattern) != std::string::npos;
}

static Config loadBasicConfig()
{
    ConfigComposer composer(
        CONFIG_DIR + "/property.txt",
        CONFIG_DIR + "/railroad.txt",
        CONFIG_DIR + "/utility.txt",
        CONFIG_DIR + "/tax.txt",
        CONFIG_DIR + "/aksi.txt",
        CONFIG_DIR + "/special.txt",
        CONFIG_DIR + "/misc.txt"
    );

    return composer.getConfig();
}

static std::map<int, int> makeRailroadRentMap(Config& config)
{
    std::map<int, int> railroadRent;

    railroadRent[1] = config.getRailroadRent(1);
    railroadRent[2] = config.getRailroadRent(2);
    railroadRent[3] = config.getRailroadRent(3);
    railroadRent[4] = config.getRailroadRent(4);

    return railroadRent;
}

static std::map<int, int> makeUtilityMultiplierMap(Config& config)
{
    std::map<int, int> utilityMultiplier;

    utilityMultiplier[1] = config.getUtilityMultiplier(1);
    utilityMultiplier[2] = config.getUtilityMultiplier(2);

    return utilityMultiplier;
}

static void buildGameFromConfig(Game& game)
{
    Config config = loadBasicConfig();
    game.setConfig(config);

    Config& gameConfig = game.getConfig();

    std::vector<std::tuple<int, std::string, std::string, std::string, std::string, int, int, std::vector<int>>> propertyConfigs =
        gameConfig.getPropertyConfigAll();

    std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> actionConfigs =
        gameConfig.getActionTileConfigAll();

    if (propertyConfigs.empty())
    {
        throw std::runtime_error(
            "Property config is empty. Fix ConfigComposer::buildConfig() so every handler calls loadConfig()."
        );
    }

    if (actionConfigs.empty())
    {
        throw std::runtime_error(
            "Action tile config is empty. Fix ConfigComposer::buildConfig() so every handler calls loadConfig()."
        );
    }

    BoardBuilder::build(
        game.getBoard(),
        propertyConfigs,
        actionConfigs,
        makeRailroadRentMap(gameConfig),
        makeUtilityMultiplierMap(gameConfig)
    );
}

static PropertyTile* getProperty(Game& game, const std::string& code)
{
    Tile* tile = game.getBoard().getTileByCode(code);
    PropertyTile* property = dynamic_cast<PropertyTile*>(tile);

    if (property == nullptr)
    {
        throw std::runtime_error("Tile " + code + " is not a PropertyTile.");
    }

    return property;
}

static int playerIdOf(Player* player)
{
    if (player == nullptr)
    {
        return -1;
    }

    return player->getId();
}

static std::string propertyCodesToString(const std::vector<PropertyTile*>& properties)
{
    std::string result;

    for (std::size_t i = 0; i < properties.size(); ++i)
    {
        if (i > 0)
        {
            result += ",";
        }

        if (properties[i] == nullptr)
        {
            result += "NULL";
        }
        else
        {
            result += properties[i]->getCode();
        }
    }

    return result;
}

static void prepareSourceGame(Game& game)
{
    buildGameFromConfig(game);

    std::vector<Player>& players = game.getPlayers();

    players.emplace_back(0, nullptr, 2000);
    players.emplace_back(1, nullptr, 1600);

    Player& playerOne = players[0];
    Player& playerTwo = players[1];

    playerOne.moveTo(2);
    playerTwo.moveTo(4);

    PropertyTile* garut = getProperty(game, "GRT");
    PropertyTile* tasikmalaya = getProperty(game, "TSK");

    garut->setOwner(&playerOne);
    playerOne.addProperty(garut);

    tasikmalaya->setOwner(&playerTwo);
    playerTwo.addProperty(tasikmalaya);

    game.getTurnManager() = TurnManager(std::vector<int>{0, 1}, 15);
    game.getTurnManager().setCurrentPlayerIndex(1);
    game.getTurnManager().setCurrentTurn(7);

    std::vector<PropertyTile*> offeredProperties;
    offeredProperties.push_back(garut);

    std::vector<PropertyTile*> requestedProperties;
    requestedProperties.push_back(tasikmalaya);

    game.getTradeManager().proposeTrade(
        &playerOne,
        &playerTwo,
        offeredProperties,
        125,
        requestedProperties,
        45
    );

    game.getLogger().log(7, playerOne.getUsername(), "TEST_TRADE", "Prepared active trade before save");
    game.setGameOver(false);
}

static void compareTurnManager(Game& source, Game& loaded)
{
    TurnManager& expected = source.getTurnManager();
    TurnManager& actual = loaded.getTurnManager();

    expectInt(
        actual.getCurrentPlayerOrderIndex(),
        expected.getCurrentPlayerOrderIndex(),
        "TurnManager current player order index is restored"
    );

    expectInt(
        actual.getCurrentPlayerIndex(),
        expected.getCurrentPlayerIndex(),
        "TurnManager current player id/index is restored"
    );

    expectInt(
        actual.getCurrentTurn(),
        expected.getCurrentTurn(),
        "TurnManager current turn is restored"
    );

    expectInt(
        actual.getMaxTurn(),
        expected.getMaxTurn(),
        "TurnManager max turn is restored"
    );

    const std::vector<int>& expectedOrder = expected.getTurnOrder();
    const std::vector<int>& actualOrder = actual.getTurnOrder();

    expectSize(
        actualOrder.size(),
        expectedOrder.size(),
        "TurnManager turn order size is restored"
    );

    const std::size_t count = std::min(actualOrder.size(), expectedOrder.size());

    for (std::size_t i = 0; i < count; ++i)
    {
        expectInt(
            actualOrder[i],
            expectedOrder[i],
            "TurnManager turn order index " + std::to_string(i) + " is restored"
        );
    }
}

static void compareTradeManager(Game& source, Game& loaded)
{
    const std::vector<TradeToPlayer*>& expectedTrades =
        source.getTradeManager().getActiveTrades();

    const std::vector<TradeToPlayer*>& actualTrades =
        loaded.getTradeManager().getActiveTrades();

    expectSize(
        actualTrades.size(),
        expectedTrades.size(),
        "TradeManager active trade count is restored"
    );

    const std::size_t count = std::min(actualTrades.size(), expectedTrades.size());

    for (std::size_t i = 0; i < count; ++i)
    {
        TradeToPlayer* expected = expectedTrades[i];
        TradeToPlayer* actual = actualTrades[i];

        const std::string prefix = "Trade " + std::to_string(i) + " ";

        check(actual != nullptr, prefix + "is not null after load");

        if (actual == nullptr || expected == nullptr)
        {
            continue;
        }

        expectInt(
            playerIdOf(actual->getProposer()),
            playerIdOf(expected->getProposer()),
            prefix + "proposer id is restored"
        );

        expectInt(
            playerIdOf(actual->getTarget()),
            playerIdOf(expected->getTarget()),
            prefix + "target id is restored"
        );

        expectInt(
            actual->getOfferedMoney(),
            expected->getOfferedMoney(),
            prefix + "offered money is restored"
        );

        expectInt(
            actual->getRequestedMoney(),
            expected->getRequestedMoney(),
            prefix + "requested money is restored"
        );

        expectString(
            propertyCodesToString(actual->getOfferedProperties()),
            propertyCodesToString(expected->getOfferedProperties()),
            prefix + "offered property codes are restored"
        );

        expectString(
            propertyCodesToString(actual->getRequestedProperties()),
            propertyCodesToString(expected->getRequestedProperties()),
            prefix + "requested property codes are restored"
        );
    }
}

static void compareBasicGameState(Game& source, Game& loaded)
{
    expectInt(
        loaded.getBoard().size(),
        source.getBoard().size(),
        "Loaded board is rebuilt from config"
    );

    expectSize(
        loaded.getPlayers().size(),
        source.getPlayers().size(),
        "Player count is restored"
    );

    expectInt(
        loaded.getPlayers()[0].getMoney(),
        source.getPlayers()[0].getMoney(),
        "Player 0 money is restored"
    );

    expectInt(
        loaded.getPlayers()[1].getMoney(),
        source.getPlayers()[1].getMoney(),
        "Player 1 money is restored"
    );

    expectInt(
        loaded.getPlayers()[0].getPosition(),
        source.getPlayers()[0].getPosition(),
        "Player 0 position is restored"
    );

    expectInt(
        loaded.getPlayers()[1].getPosition(),
        source.getPlayers()[1].getPosition(),
        "Player 1 position is restored"
    );

    PropertyTile* loadedGarut = getProperty(loaded, "GRT");
    PropertyTile* loadedTasikmalaya = getProperty(loaded, "TSK");

    expectInt(
        playerIdOf(loadedGarut->getOwner()),
        0,
        "Property GRT owner is restored"
    );

    expectInt(
        playerIdOf(loadedTasikmalaya->getOwner()),
        1,
        "Property TSK owner is restored"
    );
}

static void runTest()
{
    ensureDataDirectory();

    Game sourceGame;
    prepareSourceGame(sourceGame);

    check(sourceGame.getBoard().size() > 0, "Source game board is built from config");
    expectSize(sourceGame.getPlayers().size(), 2, "Source game has 2 players");
    expectSize(sourceGame.getTradeManager().getActiveTrades().size(), 1, "Source game has 1 active trade");

    bool saveSuccess = GameStateSaver::save(sourceGame, SAVE_PATH);
    check(saveSuccess, "GameStateSaver::save returns true");

    const std::string savedText = readWholeFile(SAVE_PATH);

    check(
        containsText(savedText, "[TURN_MANAGER]") &&
        containsText(savedText, "[/TURN_MANAGER]"),
        "Save file contains TURN_MANAGER section"
    );

    check(
        containsText(savedText, "[TRADE_MANAGER]") &&
        containsText(savedText, "[/TRADE_MANAGER]"),
        "Save file contains TRADE_MANAGER section"
    );

    Game loadedGame;
    buildGameFromConfig(loadedGame);

    bool loadSuccess = GameStateLoader::load(loadedGame, SAVE_PATH);
    check(loadSuccess, "GameStateLoader::load returns true");

    compareBasicGameState(sourceGame, loadedGame);
    compareTurnManager(sourceGame, loadedGame);
    compareTradeManager(sourceGame, loadedGame);
}

int main()
{
    std::cout << "GameStateSaver + GameStateLoader TurnManager/TradeManager Test\n";
    std::cout << "Run this from the project root directory.\n";

    printSeparator();

    try
    {
        runTest();
    }
    catch (const std::exception& e)
    {
        totalChecks++;
        failedChecks++;
        std::cout << "[FATAL] " << e.what() << '\n';
    }
    catch (...)
    {
        totalChecks++;
        failedChecks++;
        std::cout << "[FATAL] Unknown exception occurred.\n";
    }

    printSeparator();

    std::cout << "Total checks : " << totalChecks << '\n';
    std::cout << "Passed       : " << totalChecks - failedChecks << '\n';
    std::cout << "Failed       : " << failedChecks << '\n';

    if (failedChecks == 0)
    {
        std::cout << "RESULT       : ALL TESTS PASSED\n";
        return 0;
    }

    std::cout << "RESULT       : SOME TESTS FAILED\n";
    return 1;
}