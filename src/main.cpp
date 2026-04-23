#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "../include/models/GameBoard.hpp"
#include "../include/models/Tile.hpp"
#include "../include/models/Property.hpp"
#include "../include/models/Street.hpp"
#include "../include/models/Railroad.hpp"
#include "../include/models/Utility.hpp"
#include "../include/models/Player.hpp"
#include "../include/utils/SkillCard.hpp"
#include "../include/utils/CardDeck.hpp"
#include "../include/core/SkillCardManager.hpp"

#include "../include/data/ConfigParser.hpp"
#include "../include/data/TransactionLogger.hpp"
#include "../include/data/GameSaver.hpp"
#include "../include/data/GameLoader.hpp"
#include "../include/utils/FileFormatException.hpp"
#include "../include/utils/FileWriteException.hpp"

using namespace std;

static void banner(const string& title) {
    cout << "\n==================== " << title
         << " ====================\n";
}

static string playerStatusStr(PlayerStatus s) {
    switch (s) {
        case ACTIVE:   return "ACTIVE";
        case BANKRUPT: return "BANKRUPT";
        case JAILED:   return "JAILED";
    }
    return "?";
}

static string propertyStatusStr(StatusType s) {
    switch (s) {
        case BANK:      return "BANK";
        case OWNED:     return "OWNED";
        case MORTGAGED: return "MORTGAGED";
    }
    return "?";
}

static Property* findProperty(GameBoard* board, const string& code) {
    const vector<Tile*>& tiles = board->getTiles();
    for (size_t i = 0; i < tiles.size(); ++i) {
        Property* p = dynamic_cast<Property*>(tiles[i]);
        if (p != nullptr && p->getCode() == code) return p;
    }
    return nullptr;
}

static void dumpBoard(GameBoard* board) {
    cout << "Current turn: " << board->getCurrentTurnNumber()
         << " / " << board->getMaxTurn() << "\n";
    Player* cur = board->getCurrentPlayer();
    cout << "Current player: " << (cur ? cur->getUsername() : "(none)") << "\n";
    cout << "Players (in turn order):\n";
    const vector<Player*>& players = board->getPlayers();
    for (size_t i = 0; i < players.size(); ++i) {
        Player* p = players[i];
        cout << "  - " << p->getUsername()
             << " | M" << p->getMoney()
             << " | pos=" << p->getPosition()
             << " | " << playerStatusStr(p->getStatus())
             << " | hand=" << p->getHand().size();
        const vector<SkillCard*>& hand = p->getHand();
        for (size_t h = 0; h < hand.size(); ++h) {
            if (hand[h] == nullptr) continue;
            cout << " [" << hand[h]->getCardType()
                 << ":" << hand[h]->getValue()
                 << "/d" << hand[h]->getRemainingDuration() << "]";
        }
        cout << "\n";
    }
}

static void dumpOwnedProperties(GameBoard* board) {
    const vector<Tile*>& tiles = board->getTiles();
    cout << "Owned/festivaled/mortgaged properties:\n";
    for (size_t i = 0; i < tiles.size(); ++i) {
        Property* p = dynamic_cast<Property*>(tiles[i]);
        if (p == nullptr) continue;
        if (p->getStatus() == BANK && p->getFestivalMultiplier() == 1
            && p->getFestivalDuration() == 0) continue;
        cout << "  - " << p->getCode() << " | " << p->getName()
             << " | owner=" << (p->getOwner().empty() ? string("BANK") : p->getOwner())
             << " | " << propertyStatusStr(p->getStatus())
             << " | fmult=" << p->getFestivalMultiplier()
             << " fdur=" << p->getFestivalDuration();
        Street* s = dynamic_cast<Street*>(p);
        if (s != nullptr) cout << " bldg=" << s->getBuildingCount();
        cout << "\n";
    }
}

static void dumpSkillDeck(GameBoard* board) {
    CardDeck<SkillCard>* d = board->getSkillDeck();
    if (d == nullptr) {
        cout << "Skill deck: (none attached)\n";
        return;
    }
    const vector<SkillCard*>& cards = d->getDeck();
    cout << "Skill deck (" << cards.size() << " cards):";
    for (size_t i = 0; i < cards.size(); ++i) {
        if (cards[i] == nullptr) continue;
        cout << " " << cards[i]->getCardType();
    }
    cout << "\n";
}

static void dumpLog(TransactionLogger* logger) {
    vector<string> all = logger->getAll();
    cout << "TransactionLogger (" << all.size() << " entries):\n";
    for (size_t i = 0; i < all.size(); ++i) {
        cout << "  " << all[i] << "\n";
    }
}

static void printSaveFile(const string& path) {
    ifstream in(path.c_str());
    if (!in.is_open()) {
        cout << "(cannot open " << path << ")\n";
        return;
    }
    string line;
    int n = 0;
    while (getline(in, line)) {
        cout << line << "\n";
        ++n;
    }
    cout << "(" << n << " lines)\n";
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    banner("TEST 1: ConfigParser loads ../config/*");

    GameBoard board;
    ConfigParser cp("../config");
    try {
        cp.loadConfig(&board);
    } catch (FileFormatException& e) {
        cout << "FileFormatException: " << e.what() << "\n";
        return 1;
    }

    const vector<Tile*>& tiles = board.getTiles();
    cout << "Tiles loaded: " << tiles.size() << " (expected 40)\n";
    cout << "Sample codes:";
    int checked = 0;
    for (size_t i = 0; i < tiles.size() && checked < 10; ++i) {
        if (tiles[i] == nullptr) continue;
        cout << " [" << tiles[i]->getPosition() + 1 << ":"
             << tiles[i]->getCode() << "]";
        ++checked;
    }
    cout << "\n";

    Property* jkt = findProperty(&board, "JKT");
    cout << "Lookup JKT: "
         << (jkt ? jkt->getName() + " @ pos " + to_string(jkt->getPosition())
                 : string("NOT FOUND")) << "\n";
    Property* pln = findProperty(&board, "PLN");
    cout << "Lookup PLN: "
         << (pln ? pln->getName() + " @ pos " + to_string(pln->getPosition())
                 : string("NOT FOUND")) << "\n";

    cout << "MAX_TURN     = " << cp.getMaxTurn() << "\n";
    cout << "SALDO_AWAL   = " << cp.getStartBalance() << "\n";
    cout << "GO_SALARY    = " << cp.getGoSalary() << "\n";
    cout << "JAIL_FINE    = " << cp.getJailFine() << "\n";

    banner("TEST 2: TransactionLogger basic ops");

    TransactionLogger logger;
    logger.log(1, "Kebin",   "DADU", "Lempar: 3+4=7 mendarat di Bogor (BGR)");
    logger.log(1, "Kebin",   "BELI", "Beli Bogor (BGR) seharga M100");
    logger.log(2, "Stewart", "DADU", "Lempar: 4+4=8 mendarat di BGR");
    logger.log(2, "Stewart", "SEWA", "Bayar M6 ke Kebin (BGR, L0)");
    logger.log(2, "Gro",     "DADU", "Lempar: 6+6=12 mendarat di Jakarta (JKT)");
    logger.log(2, "Gro",     "BELI", "Beli Jakarta (JKT) seharga M350");

    cout << "getCount()  = " << logger.getCount() << "\n";
    cout << "getRecent(3):\n";
    vector<string> recent = logger.getRecent(3);
    for (size_t i = 0; i < recent.size(); ++i) cout << "  " << recent[i] << "\n";
    dumpLog(&logger);

    banner("TEST 3: Set up game state before save");

    Player* p1 = new Player("Kebin",   cp.getStartBalance() - 100);
    Player* p2 = new Player("Stewart", cp.getStartBalance() + 300);
    Player* p3 = new Player("Gro",     cp.getStartBalance());
    board.addPlayer(p1);
    board.addPlayer(p2);
    board.addPlayer(p3);

    p1->setPosition(6);
    p2->setPosition(3);
    p3->setPosition(37);
    p3->setStatus(JAILED);

    SkillCardManager skillMgr(3);
    skillMgr.initDeck();
    board.setSkillDeck(&skillMgr.getDeck());

    vector<Player*> order;
    order.push_back(p1);
    order.push_back(p2);
    order.push_back(p3);
    skillMgr.distributeCardToAll(order);
    skillMgr.distributeCardToAll(order);

    board.setMaxTurn(cp.getMaxTurn());
    board.setCurrentTurnNumber(4);

    Property* bgr = findProperty(&board, "BGR");
    if (bgr != nullptr) { bgr->setOwner("Kebin"); bgr->setStatus(OWNED); }
    Property* jktp = findProperty(&board, "JKT");
    if (jktp != nullptr) {
        jktp->setOwner("Gro");
        jktp->setStatus(OWNED);
        jktp->setFestivalMultiplier(2);
        jktp->setFestivalDuration(3);
        Street* js = dynamic_cast<Street*>(jktp);
        if (js != nullptr) js->setBuildingCount("2");
    }
    Property* grt = findProperty(&board, "GRT");
    if (grt != nullptr) { grt->setOwner("Stewart"); grt->setStatus(MORTGAGED); }
    Property* gbr = findProperty(&board, "GBR");
    if (gbr != nullptr) { gbr->setOwner("Kebin"); gbr->setStatus(OWNED); }

    board.setCurrentPlayerByUsername("Stewart");

    cout << "BEFORE SAVE:\n";
    dumpBoard(&board);
    dumpOwnedProperties(&board);
    dumpSkillDeck(&board);

    banner("TEST 4: GameSaver::save");

    GameSaver saver;
    const string savePath = "../data/test_save.txt";
    try {
        bool ok = saver.save(&board, &logger, savePath);
        cout << "save() returned " << (ok ? "true" : "false") << "\n";
    } catch (FileWriteException& e) {
        cout << "FileWriteException: " << e.what() << "\n";
        return 1;
    }
    cout << "\n--- " << savePath << " ---\n";
    printSaveFile(savePath);

    banner("TEST 5: GameLoader::validate");

    GameLoader gl;
    cout << "validate('" << savePath << "') = "
         << (gl.validate(savePath) ? "true" : "false") << "\n";
    cout << "validate('../config/misc.txt')   = "
         << (gl.validate("../config/misc.txt") ? "true" : "false") << "\n";
    cout << "validate('../config/nope.txt')   = "
         << (gl.validate("../config/nope.txt") ? "true" : "false") << "\n";

    banner("TEST 6: GameLoader::loadSave roundtrip");

    GameBoard board2;
    ConfigParser cp2("../config");
    try {
        cp2.loadConfig(&board2);
    } catch (FileFormatException& e) {
        cout << "FileFormatException in fresh load: " << e.what() << "\n";
        return 1;
    }

    SkillCardManager skillMgr2(3);
    skillMgr2.initDeck();
    board2.setSkillDeck(&skillMgr2.getDeck());

    TransactionLogger logger2;

    try {
        bool ok = gl.loadSave(savePath, &board2, &logger2);
        cout << "loadSave() returned " << (ok ? "true" : "false") << "\n";
    } catch (FileFormatException& e) {
        cout << "FileFormatException: " << e.what() << "\n";
        return 1;
    }

    cout << "\nAFTER LOAD:\n";
    dumpBoard(&board2);
    dumpOwnedProperties(&board2);
    dumpSkillDeck(&board2);
    cout << "\n";
    dumpLog(&logger2);

    banner("TEST 7: Roundtrip save ");

    const string savePath2 = "../data/test_save2.txt";
    try {
        saver.save(&board2, &logger2, savePath2);
    } catch (FileWriteException& e) {
        cout << "FileWriteException: " << e.what() << "\n";
        return 1;
    }

    ifstream a(savePath.c_str()), b(savePath2.c_str());
    string la, lb;
    int lineNum = 0;
    bool identical = true;
    while (true) {
        bool gotA = static_cast<bool>(getline(a, la));
        bool gotB = static_cast<bool>(getline(b, lb));
        if (!gotA && !gotB) break;
        ++lineNum;
        if (gotA != gotB || la != lb) {
            identical = false;
            cout << "  diff at line " << lineNum << ":\n";
            cout << "    A: " << (gotA ? la : string("<eof>")) << "\n";
            cout << "    B: " << (gotB ? lb : string("<eof>")) << "\n";
        }
    }
    cout << "Round-trip save identical: " << (identical ? "YES" : "NO") << "\n";

    banner("TEST 8: bad save file");

    const string badPath = "../data/bad_save.txt";
    {
        ofstream bad(badPath.c_str());
        bad << "not_a_number alsonot\n";
    }
    try {
        GameBoard board3;
        ConfigParser cp3("../config");
        cp3.loadConfig(&board3);
        TransactionLogger logger3;
        gl.loadSave(badPath, &board3, &logger3);
        cout << "lawll \n";
    } catch (FileFormatException& e) {
        cout << "caught as expected: " << e.what() << "\n";
    }

    banner("DONE");

    return 0;
}
