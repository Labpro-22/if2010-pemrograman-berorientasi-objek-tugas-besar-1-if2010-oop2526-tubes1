#include "../lib/raylib/include/raylib.h"
#include "views/GUIManager.hpp"
#include "views/screens/GameScreen.hpp"
#include "views/screens/MainMenuScreen.hpp"
#include "views/screens/WinScreen.hpp"

#include "core/GameMaster/GameMaster.hpp"
#include "core/GameState/GameState.hpp"
#include "core/Board/Board.hpp"
#include "core/Player/Player.hpp"
#include "core/Bank/Bank.hpp"
#include "core/Dice/Dice.hpp"
#include "core/AuctionManager/AuctionManager.hpp"
#include "core/Card/CardDeck.hpp"
#include "core/Card/Card.hpp"
#include "core/utils/TransactionLogger.hpp"
#include "core/utils/ConfigLoader.hpp"
#include "core/Property/PropertyFactory.hpp"

#include <memory>
#include <vector>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
//  buildBoard()
//
//  Membuat Board 40 petak. Property* di-assign ke tile via setProperty()
//  berdasarkan code matching. Ownership Property tetap di `properties`
//  (vector<unique_ptr<Property>> yang hidup di main()).
// ─────────────────────────────────────────────────────────────────────────────
static Board* buildBoard(
    const std::vector<std::unique_ptr<Property>>& properties,
    int goSalary,
    int jailFine,
    CardDeck<Card>* chanceDeck,
    CardDeck<Card>* communityDeck)
{
    // Helper: cari Property* berdasarkan code
    auto findProp = [&](const std::string& code) -> Property* {
        for (auto& p : properties)
            if (p->getCode() == code) return p.get();
        return nullptr;
    };

    // Helper: buat PropertyTile dan langsung pasang Property*
    auto makeStreet = [&](int id, const std::string& code, TileName name) -> StreetTile* {
        auto* tile = new StreetTile(id, code, TileType::STREET, name, code);
        tile->setProperty(findProp(code));
        return tile;
    };
    auto makeRailroad = [&](int id, const std::string& code, TileName name) -> RailroadTile* {
        auto* tile = new RailroadTile(id, code, TileType::RAILROAD, name, code);
        tile->setProperty(findProp(code));
        return tile;
    };
    auto makeUtility = [&](int id, const std::string& code, TileName name) -> UtilityTile* {
        auto* tile = new UtilityTile(id, code, TileType::UTILITY, name, code);
        tile->setProperty(findProp(code));
        return tile;
    };

    std::vector<Tile*> tiles;
    tiles.reserve(40);

    // ── BOTTOM ROW (idx 0–10) ─────────────────────────────────────────────
    tiles.push_back(new GoTile        ( 0, "GO",  TileType::SPECIAL,   TileName::GO,         "GO",  goSalary));
    tiles.push_back(makeStreet        ( 1, "GRT", TileName::GARUT));
    tiles.push_back(new CardTile      ( 2, "DNU", TileType::CARD,      TileName::COMMON_FUND,"DNU", communityDeck));
    tiles.push_back(makeStreet        ( 3, "TSK", TileName::TASIKMALAYA));
    tiles.push_back(new TaxTile       ( 4, "PPH", TileType::TAX,       TileName::INCOME_TAX, "PPH"));
    tiles.push_back(makeRailroad      ( 5, "GBR", TileName::ST_GAMBIR));
    tiles.push_back(makeStreet        ( 6, "BGR", TileName::BOGOR));
    tiles.push_back(new FestivalTile  ( 7, "FES", TileType::FESTIVAL,  TileName::FESTIVAL,   "FES"));
    tiles.push_back(makeStreet        ( 8, "DPK", TileName::DEPOK));
    tiles.push_back(makeStreet        ( 9, "BKS", TileName::BEKASI));
    tiles.push_back(new JailTile      (10, "PEN", TileType::SPECIAL,   TileName::JAIL,       "PEN", {}, {}, jailFine));

    // ── LEFT COLUMN (idx 11–19) ───────────────────────────────────────────
    tiles.push_back(makeStreet        (11, "SBY", TileName::SURABAYA));
    tiles.push_back(makeStreet        (12, "SMG", TileName::SEMARANG));
    tiles.push_back(new CardTile      (13, "DNU", TileType::CARD,      TileName::COMMON_FUND,"DNU", communityDeck));
    tiles.push_back(makeStreet        (14, "MAL", TileName::MALANG));
    tiles.push_back(makeRailroad      (15, "STB", TileName::ST_BANDUNG));
    tiles.push_back(makeStreet        (16, "YOG", TileName::YOGYAKARTA));
    tiles.push_back(makeStreet        (17, "SOL", TileName::SOLO));
    tiles.push_back(makeUtility       (18, "PLN", TileName::PLN));
    tiles.push_back(makeStreet        (19, "MGL", TileName::MAGELANG));

    // ── TOP ROW (idx 20–30) ───────────────────────────────────────────────
    tiles.push_back(new FreeParkingTile(20,"BBP", TileType::SPECIAL,   TileName::FREE_PARK,  "BBP"));
    tiles.push_back(makeStreet         (21,"MKS", TileName::MAKASSAR));
    tiles.push_back(new CardTile       (22,"KSP", TileType::CARD,      TileName::CHANCE,     "KSP", chanceDeck));
    tiles.push_back(makeStreet         (23,"BLP", TileName::BALIKPAPAN));
    tiles.push_back(makeStreet         (24,"MND", TileName::MANADO));
    tiles.push_back(makeRailroad       (25,"TUG", TileName::ST_TUGU));
    tiles.push_back(makeStreet         (26,"PLB", TileName::PALEMBANG));
    tiles.push_back(makeStreet         (27,"PKB", TileName::PEKANBARU));
    tiles.push_back(makeUtility        (28,"PAM", TileName::PAM));
    tiles.push_back(makeStreet         (29,"MED", TileName::MEDAN));
    tiles.push_back(new GoToJail       (30,"PPJ", TileType::SPECIAL,   TileName::GO_TO_JAIL, "PPJ"));

    // ── RIGHT COLUMN (idx 31–39) ──────────────────────────────────────────
    tiles.push_back(makeStreet         (31,"BDG", TileName::BANDUNG));
    tiles.push_back(makeStreet         (32,"DEN", TileName::DENPASAR));
    tiles.push_back(new FestivalTile   (33,"FES", TileType::FESTIVAL,  TileName::FESTIVAL,   "FES"));
    tiles.push_back(makeStreet         (34,"MTR", TileName::MATARAM));
    tiles.push_back(makeRailroad       (35,"GUB", TileName::ST_GUBENG));
    tiles.push_back(new CardTile       (36,"KSP", TileType::CARD,      TileName::CHANCE,     "KSP", chanceDeck));
    tiles.push_back(makeStreet         (37,"JKT", TileName::JAKARTA));
    tiles.push_back(new TaxTile        (38,"PBM", TileType::TAX,       TileName::LUX_GOODS,  "PBM"));
    tiles.push_back(makeStreet         (39,"IKN", TileName::IKN));

    return new Board(tiles, (int)tiles.size());
}


// ─────────────────────────────────────────────────────────────────────────────
//  main()
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    std::string title = "Nimonspoli";
    GUIManager gui(1920, 1080, title, 60);

    MainMenuScreen* menuScreen = new MainMenuScreen();
    GameScreen*     gameScreen = new GameScreen();
    WinScreen*      winScreen  = new WinScreen();

    gui.setScreen(menuScreen);

    // ── Objek game — di-reset setiap New Game ────────────────────────────
    GameMaster*     gameMaster    = nullptr;
    Bank*           bank          = nullptr;
    Dice*           dice          = nullptr;
    AuctionManager* auctionMgr    = nullptr;
    CardDeck<Card>* chanceDeck    = nullptr;
    CardDeck<Card>* communityDeck = nullptr;
    CardDeck<Card>* skillDeck     = nullptr;
    TransactionLogger* logger     = nullptr;
    Board*          board         = nullptr;
    std::vector<Player*> players;
    std::vector<std::unique_ptr<Property>> properties; // ownership semua Property*

    // Helper cleanup
    auto cleanupGame = [&]() {
        delete gameMaster;   gameMaster    = nullptr;
        delete bank;         bank          = nullptr;
        delete dice;         dice          = nullptr;
        delete auctionMgr;   auctionMgr    = nullptr;
        delete chanceDeck;   chanceDeck    = nullptr;
        delete communityDeck;communityDeck = nullptr;
        delete skillDeck;    skillDeck     = nullptr;
        delete logger;       logger        = nullptr;
        delete board;        board         = nullptr;
        for (auto* p : players) delete p;
        players.clear();
        properties.clear(); // unique_ptr auto-delete
    };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // ── Transisi: Menu → Game ─────────────────────────────────────────
        if (menuScreen->isReadyToStart()) {
            auto setup = menuScreen->getSetup();
            menuScreen->resetReady();
            cleanupGame();

            // Load config
            ConfigLoader cfg("config");
            auto propData      = cfg.loadProperties();
            auto railroadRent  = cfg.loadRailroad();
            auto utilityFactor = cfg.loadUtility();
            auto specialCfg    = cfg.loadSpecial();
            auto miscCfg       = cfg.loadMisc();

            // Buat semua Property via factory
            properties = PropertyFactory::createProperties(
                propData, railroadRent, utilityFactor);

            // Buat entitas
            bank          = new Bank(32, 12);
            dice          = new Dice();
            auctionMgr    = new AuctionManager();
            logger        = new TransactionLogger();
            chanceDeck    = new CardDeck<Card>();
            communityDeck = new CardDeck<Card>();
            skillDeck     = new CardDeck<Card>();

            // Buat pemain
            int count = std::max(2, std::min(4, setup.playerCount));
            for (int i = 0; i < count; ++i) {
                std::string name = (i < (int)setup.names.size() && !setup.names[i].empty())
                                 ? setup.names[i]
                                 : ("Pemain" + std::to_string(i + 1));
                players.push_back(new Player(name, miscCfg.initialBalance));
            }

            // Buat Board — property sudah ter-assign via setProperty()
            board = buildBoard(properties, specialCfg.goSalary, specialCfg.jailFine,
                               chanceDeck, communityDeck);

            // Buat GameState
            GameState gs(
                miscCfg.maxTurn, players, board, bank, dice,
                auctionMgr, chanceDeck, communityDeck, skillDeck, logger
            );

            // Buat GameMaster
            gameMaster = new GameMaster(gs);

            // Hubungkan ke GUI
            gui.setGameMaster(gameMaster);
            gameScreen->setGUIManager(&gui);
            gameScreen->setPlayerCount(count);
            gui.setScreen(gameScreen);

            gameMaster->beginTurn();
        }

        // ── Transisi: Game → Win ──────────────────────────────────────────
        if (gameScreen->isGameOver()) {
            std::vector<PlayerResult> results;
            for (int i = 0; i < (int)players.size(); ++i) {
                PlayerResult r;
                r.username = players[i]->getUsername();
                r.money    = players[i]->getBalance();
                r.color    = gameScreen->playerColors[i];
                results.push_back(r);
            }
            winScreen->setResults(results, WinScenario::MAX_TURN);
            gameScreen->gameOver = false;
            gui.setGameMaster(nullptr);
            gui.setScreen(winScreen);
        }

        // ── Transisi: Win → Menu ──────────────────────────────────────────
        if (winScreen->goToMainMenu()) {
            winScreen->reset();
            cleanupGame();
            gui.setScreen(menuScreen);
        }
        if (winScreen->goToExit()) break;

        // ── Game logic: flush command → sync dice ─────────────────────────
        gui.flushCommands();

        if (gui.getCurrentScreen()) {
            auto* gs = dynamic_cast<GameScreen*>(gui.getCurrentScreen());
            if (gs) gs->syncDiceResult();
        }

        // ── Update + Render ───────────────────────────────────────────────
        BeginDrawing();
        if (gui.getCurrentScreen()) {
            gui.getCurrentScreen()->update(dt);
            gui.getCurrentScreen()->render(gui.getWindow());
        }
        EndDrawing();
    }

    // ── Cleanup final ─────────────────────────────────────────────────────
    cleanupGame();
    gui.setScreen(nullptr);
    delete menuScreen;
    delete gameScreen;
    delete winScreen;
    return 0;
}