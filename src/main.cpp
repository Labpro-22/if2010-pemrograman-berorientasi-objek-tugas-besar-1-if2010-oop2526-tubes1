// main.cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "../include/models/Player.hpp"
#include "../include/models/GameBoard.hpp"
#include "../include/models/Tile.hpp"
#include "../include/models/GoTile.hpp"
#include "../include/models/GoToJailTile.hpp"
#include "../include/models/JailTile.hpp"
#include "../include/models/FreeParkingTile.hpp"
#include "../include/models/ChanceTile.hpp"
#include "../include/models/CommunityChestTile.hpp"
#include "../include/models/TaxTile.hpp"
#include "../include/models/FestivalTile.hpp"
#include "../include/models/Street.hpp"
#include "../include/models/Railroad.hpp"
#include "../include/models/Utility.hpp"
#include "../include/utils/Dice.hpp"
#include "../include/utils/CardDeck.hpp"
#include "../include/utils/ActionCard.hpp"
#include "../include/utils/SkillCard.hpp"
#include "../include/models/JailTile.hpp"
#include "../include/core/GameContext.hpp"
#include "../include/core/MovementHandler.hpp"
#include "../include/core/TurnManager.hpp"
#include "../include/core/SkillCardManager.hpp"

using namespace std;

// ── helper print ──────────────────────────────────────────
void printSeparator(const string& title) {
    cout << "\n========================================\n";
    cout << "  " << title << "\n";
    cout << "========================================\n";
}

void printPlayerStatus(Player* p) {
    cout << "[" << p->getUsername() << "] "
         << "Uang: M" << p->getMoney()
         << " | Pos: " << p->getPosition()
         << " | Status: ";
    switch (p->getStatus()) {
        case ACTIVE:   cout << "ACTIVE";   break;
        case JAILED:   cout << "JAILED";   break;
        case BANKRUPT: cout << "BANKRUPT"; break;
    }
    cout << " | Kartu: " << p->getHand().size() << "\n";
}

// ── setup board minimal 40 petak ─────────────────────────
GameBoard* setupBoard(CardDeck<ActionCard>* chanceDeck,
                      CardDeck<ActionCard>* communityDeck,
                      int goSalary, int jailFine) {
    GameBoard* board = new GameBoard();

    for (int i = 0; i < 40; i++) {
        Tile* tile = nullptr;

        switch (i) {
            case 0:
                tile = new GoTile(i, "GO", "DEFAULT", goSalary);
                break;
            case 3:
                tile = new CommunityChestTile(i, "Dana Umum", "DEFAULT", communityDeck);
                break;
            case 5:
                tile = new TaxTile(i, "PPH", "DEFAULT", PPH, 150, 0.10);
                break;
            case 7:
                tile = new ChanceTile(i, "Kesempatan", "DEFAULT", chanceDeck);
                break;
            case 10:
                tile = new JailTile(i, "Penjara", "DEFAULT", jailFine);
                break;
            case 17:
                tile = new CommunityChestTile(i, "Dana Umum", "DEFAULT", communityDeck);
                break;
            case 20:
                tile = new FreeParkingTile(i, "Bebas Parkir", "DEFAULT");
                break;
            case 22:
                tile = new ChanceTile(i, "Kesempatan", "DEFAULT", chanceDeck);
                break;
            case 30:
                tile = new GoToJailTile(i, "Pergi ke Penjara", "DEFAULT");
                break;
            case 33:
                tile = new FestivalTile(i, "Festival", "DEFAULT");
                break;
            case 38:
                tile = new TaxTile(i, "PBM", "DEFAULT", PBM, 200, 0.0);
                break;

            // Street sampel
            case 1: {
                Street* s = new Street(i, "Garut", "COKLAT",
                                       COKELAT, 60, 40, 20, 50);
                map<int,int> rt = {{0,2},{1,10},{2,30},{3,90},{4,160},{5,250}};
                s->setRentTable(rt);
                tile = s;
                break;
            }
            case 2: {
                Street* s = new Street(i, "Tasikmalaya", "COKLAT",
                                       COKELAT, 60, 40, 20, 50);
                map<int,int> rt = {{0,4},{1,20},{2,60},{3,180},{4,320},{5,450}};
                s->setRentTable(rt);
                tile = s;
                break;
            }

            // Railroad sampel
            case 6: {
                Railroad* r = new Railroad(i, "Stasiun Gambir", "DEFAULT", 200, 100);
                map<int,int> rt = {{1,25},{2,50},{3,100},{4,200}};
                r->setRentTable(rt);
                tile = r;
                break;
            }

            // Utility sampel
            case 12: {
                Utility* u = new Utility(i, "PLN", "ABU_ABU", 150, 75);
                map<int,int> mt = {{1,4},{2,10}};
                u->setMultiplierTable(mt);
                tile = u;
                break;
            }

            default:
                // Tile kosong — pakai FreeParkingTile sebagai placeholder
                tile = new FreeParkingTile(i, "Petak " + to_string(i), "DEFAULT");
                break;
        }

        board->addTile(tile);
    }

    return board;
}

// ── setup deck kartu aksi ────────────────────────────────
void setupActionDecks(CardDeck<ActionCard>* chanceDeck,
                      CardDeck<ActionCard>* communityDeck) {
    // Chance: mundur 3, masuk penjara, pergi stasiun terdekat
    chanceDeck->addCard(new ChanceCard(ActionCardType::MOVE, -3, "Mundur 3 petak."));
    chanceDeck->addCard(new ChanceCard(ActionCardType::GO_TO_JAIL, 0, "Masuk Penjara."));
    chanceDeck->addCard(new ChanceCard(ActionCardType::TELEPORT, 6, "Pergi ke stasiun terdekat."));

    // Community: ulang tahun, biaya dokter, nyaleg
    communityDeck->addCard(new CommunityCard(ActionCardType::COLLECT_FROM_ALL, 100,
                                              "Hari ulang tahun. Dapat M100 dari setiap pemain."));
    communityDeck->addCard(new CommunityCard(ActionCardType::PAY_MONEY, 700,
                                              "Biaya dokter. Bayar M700."));
    communityDeck->addCard(new CommunityCard(ActionCardType::PAY_TO_ALL, 200,
                                              "Mau nyaleg. Bayar M200 ke setiap pemain."));
}

// ══════════════════════════════════════════════════════════
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "=== NIMONSPOLI TEST SUITE ===\n";

    // ── konstanta dari config (placeholder) ───────────────
    const int GO_SALARY     = 200;
    const int JAIL_FINE     = 50;
    const int START_BALANCE = 1500;
    const int MAX_TURN      = 10;
    const int MAX_HAND      = 3;

    // ── setup deck ────────────────────────────────────────
    CardDeck<ActionCard>* chanceDeck    = new CardDeck<ActionCard>();
    CardDeck<ActionCard>* communityDeck = new CardDeck<ActionCard>();
    setupActionDecks(chanceDeck, communityDeck);

    // ── setup board ───────────────────────────────────────
    GameBoard* board = setupBoard(chanceDeck, communityDeck, GO_SALARY, JAIL_FINE);

    // ── buat pemain ───────────────────────────────────────
    Player* p1 = new Player("Kebin",   START_BALANCE);
    Player* p2 = new Player("Stewart", START_BALANCE);
    Player* p3 = new Player("Gro",     START_BALANCE);

    board->addPlayer(p1);
    board->addPlayer(p2);
    board->addPlayer(p3);

    vector<Player*> allPlayers = {p1, p2, p3};

    // ── setup komponen core ───────────────────────────────
    Dice dice;
    SkillCardManager skillMgr(MAX_HAND);
    skillMgr.initDeck();

    // GameContext — isi dulu yang bisa, movementHandler menyusul
    GameContext* ctx = new GameContext();
    ctx->board           = board;
    ctx->turnManager     = nullptr; // isi setelah TurnManager dibuat
    ctx->skillCardManager = &skillMgr;
    ctx->lastDice        = &dice;
    ctx->allPlayers      = allPlayers;

    MovementHandler movHandler(board, ctx, 40, GO_SALARY, JAIL_FINE);
    ctx->movementHandler = &movHandler;

    TurnManager turnMgr(allPlayers, MAX_TURN);
    ctx->turnManager = &turnMgr;

    // ══════════════════════════════════════════════════════
    // TEST 1 — Player dasar
    // ══════════════════════════════════════════════════════
    printSeparator("TEST 1: Player Dasar");

    cout << "Status awal:\n";
    for (Player* p : allPlayers) printPlayerStatus(p);

    // operator+=
    *p1 += 300;
    cout << "\nKebin terima M300 (gaji GO):\n";
    printPlayerStatus(p1);

    // operator-= normal
    *p2 -= 100;
    cout << "\nStewart bayar sewa M100:\n";
    printPlayerStatus(p2);

    // operator-= dengan shield
    p3->activateShield();
    *p3 -= 500;
    cout << "\nGro kena tagihan M500 tapi shield aktif:\n";
    printPlayerStatus(p3);
    cout << "Shield masih aktif: " << (p3->isShieldActive() ? "YA" : "TIDAK") << "\n";

    // payVoluntary — tidak kena shield
    p3->activateShield();
    p3->payVoluntary(200);
    cout << "\nGro beli properti M200 (voluntary, shield tidak block):\n";
    printPlayerStatus(p3);
    cout << "Shield masih aktif: " << (p3->isShieldActive() ? "YA" : "TIDAK") << "\n";

    // ══════════════════════════════════════════════════════
    // TEST 2 — Dice
    // ══════════════════════════════════════════════════════
    printSeparator("TEST 2: Dice");

    dice.rollRandom();
    cout << "Roll random: " << dice.getDie1() << " + " << dice.getDie2()
         << " = " << dice.getTotal()
         << (dice.isDouble() ? " (DOUBLE!)" : "") << "\n";

    dice.setManual(6, 6);
    cout << "Manual 6+6: total=" << dice.getTotal()
         << ", double=" << (dice.isDouble() ? "YA" : "TIDAK") << "\n";

    dice.setManual(3, 4);
    cout << "Manual 3+4: total=" << dice.getTotal() << "\n";

    // ══════════════════════════════════════════════════════
    // TEST 3 — MovementHandler
    // ══════════════════════════════════════════════════════
    printSeparator("TEST 3: MovementHandler");

    // Gerak normal
    p1->setPosition(0);
    dice.setManual(3, 4);
    cout << "Kebin di pos 0, gerak " << dice.getTotal() << " langkah:\n";
    movHandler.movePlayer(p1, dice.getTotal());
    printPlayerStatus(p1);

    // Pass GO
    p2->setPosition(36);
    dice.setManual(4, 4);
    int moneyBefore = p2->getMoney();
    cout << "\nStewart di pos 36, gerak " << dice.getTotal() << " (harusnya pass GO):\n";
    movHandler.movePlayer(p2, dice.getTotal());
    cout << "Uang sebelum: M" << moneyBefore << " | Setelah: M" << p2->getMoney()
         << " | Dapat gaji GO: " << (p2->getMoney() > moneyBefore ? "YA" : "TIDAK") << "\n";
    printPlayerStatus(p2);

    // sendToJail
    cout << "\nGro dikirim ke penjara:\n";
    movHandler.sendToJail(p3);
    printPlayerStatus(p3);

    // ══════════════════════════════════════════════════════
    // TEST 4 — JailTile
    // ══════════════════════════════════════════════════════
    printSeparator("TEST 4: Jail Mechanism");

    // Coba escape dengan double
    p3->setStatus(JAILED);
    p3->resetJailTurns();

    cout << "Gro di penjara, coba escape:\n";
    for (int attempt = 1; attempt <= 4; attempt++) {
        if (p3->getStatus() != JAILED) break;

        dice.rollRandom();
        cout << "  Percobaan " << attempt << ": "
             << dice.getDie1() << "+" << dice.getDie2() << "=" << dice.getTotal();

        JailResult result = movHandler.handleJailTurn(p3, dice);
        switch (result) {
            case ESCAPED_DOUBLE:
                cout << " → BEBAS (double)!\n";
                break;
            case STILL_JAILED:
                cout << " → Masih di penjara (giliran ke-"
                     << p3->getJailTurnsRemaining() << ")\n";
                break;
            case FORCED_OUT:
                cout << " → Wajib bayar denda M" << JAIL_FINE << " dan keluar\n";
                break;
        }
    }
    printPlayerStatus(p3);

    // payFine
    p3->setStatus(JAILED);
    p3->resetJailTurns();
    JailTile* jailTile = dynamic_cast<JailTile*>(board->getTileAt(10));
    if (jailTile != nullptr) {
        cout << "\nGro bayar denda langsung:\n";
        jailTile->payFine(p3);
        printPlayerStatus(p3);
    }

    // ══════════════════════════════════════════════════════
    // TEST 5 — TurnManager
    // ══════════════════════════════════════════════════════
    printSeparator("TEST 5: TurnManager");

    cout << "Urutan giliran: ";
    for (const string& u : turnMgr.getTurnOrder()) cout << u << " ";
    cout << "\n";

    for (int i = 0; i < 5; i++) {
        Player* cur = turnMgr.getCurrentPlayer();
        if (cur == nullptr) break;
        cout << "Turn " << turnMgr.getCurrentTurnNumber()
             << " — Giliran: " << cur->getUsername() << "\n";
        turnMgr.resetTurnFlags(cur);
        turnMgr.advanceToNextPlayer();
    }

    // Skip bankrupt
    cout << "\nStewart bangkrut — harusnya diskip:\n";
    p2->setStatus(BANKRUPT);
    for (int i = 0; i < 4; i++) {
        Player* cur = turnMgr.getCurrentPlayer();
        if (cur == nullptr) { cout << "Semua bangkrut!\n"; break; }
        cout << "Giliran: " << cur->getUsername() << "\n";
        turnMgr.advanceToNextPlayer();
    }
    p2->setStatus(ACTIVE); // reset untuk test berikutnya

    // ══════════════════════════════════════════════════════
    // TEST 6 — SkillCardManager
    // ══════════════════════════════════════════════════════
    printSeparator("TEST 6: SkillCardManager");

    // Distribute ke semua
    cout << "Distribusi kartu ke semua pemain:\n";
    skillMgr.distributeCardToAll(allPlayers);
    for (Player* p : allPlayers) {
        cout << p->getUsername() << " punya " << p->getHand().size() << " kartu: ";
        for (SkillCard* c : p->getHand()) cout << c->getDescription() << " | ";
        cout << "\n";
    }

    // Use card
    if (!p1->getHand().empty()) {
        cout << "\nKebin pakai kartu index 0 (" 
             << p1->getHand()[0]->getDescription() << "):\n";
        skillMgr.useCard(p1, 0, ctx);
        cout << "Sisa kartu Kebin: " << p1->getHand().size() << "\n";
        printPlayerStatus(p1);
    }

    // Drop card
    if (!p2->getHand().empty()) {
        cout << "\nStewart drop kartu index 0 ("
             << p2->getHand()[0]->getDescription() << "):\n";
        skillMgr.dropCard(p2, 0);
        cout << "Sisa kartu Stewart: " << p2->getHand().size() << "\n";
    }

    // Overflow — dapat kartu ke-4
    cout << "\nTest overflow kartu (isi tangan sampai penuh dulu):\n";
    while (p3->getHand().size() < 3) skillMgr.distributeCardTo(p3);
    cout << "Gro punya " << p3->getHand().size() << " kartu\n";
    SkillCard* overflow = skillMgr.distributeCardTo(p3);
    if (overflow != nullptr) {
        cout << "OVERFLOW — Gro dapat kartu ke-4: "
             << overflow->getDescription() << "\n";
        cout << "Gro harus drop 1 kartu (simulasi drop index 0)\n";
        skillMgr.dropCard(p3, 0);
        cout << "Sisa kartu Gro: " << p3->getHand().size() << "\n";
    }

    // ══════════════════════════════════════════════════════
    // TEST 7 — Tile Landing
    // ══════════════════════════════════════════════════════
    printSeparator("TEST 7: Tile Landing");

        // Validasi apakah onLand memengaruhi posisi pemain
        cout << "Validasi dampak onLand ke posisi pemain:\n";
        FreeParkingTile* fpPos = dynamic_cast<FreeParkingTile*>(board->getTileAt(20));
        GoToJailTile* gtjPos = dynamic_cast<GoToJailTile*>(board->getTileAt(30));

        // Case A: landing di tile tanpa perpindahan posisi
        p1->setStatus(ACTIVE);
        p1->setPosition(20);
        int posBefore = p1->getPosition();
        if (fpPos != nullptr) fpPos->onLand(p1, ctx);
        cout << "- FreeParking: pos sebelum=" << posBefore
            << ", sesudah=" << p1->getPosition()
            << " | Posisi berubah: " << (p1->getPosition() != posBefore ? "YA" : "TIDAK")
            << " (harusnya TIDAK)\n";

        // Case B: landing di tile yang memaksa perpindahan posisi
        p2->setStatus(ACTIVE);
        p2->setPosition(30);
        posBefore = p2->getPosition();
        if (gtjPos != nullptr) gtjPos->onLand(p2, ctx);
        cout << "- GoToJail: pos sebelum=" << posBefore
            << ", sesudah=" << p2->getPosition()
            << " | Posisi berubah: " << (p2->getPosition() != posBefore ? "YA" : "TIDAK")
            << " (harusnya YA), status="
            << (p2->getStatus() == JAILED ? "JAILED" : "BUKAN JAILED") << "\n";

    // GO Tile
    cout << "Kebin mendarat di GO:\n";
    int before = p1->getMoney();
    GoTile* goTile = dynamic_cast<GoTile*>(board->getTileAt(0));
    if (goTile) goTile->onLand(p1, ctx);
    cout << "Dapat gaji: M" << (p1->getMoney() - before) << "\n";
    printPlayerStatus(p1);

    // Street — beli manual untuk test sewa
    cout << "\nTest sewa Street:\n";
    Street* garut = dynamic_cast<Street*>(board->getTileAt(1));
    if (garut != nullptr) {
        garut->setOwner(p1->getUsername());
        garut->setStatus(OWNED);
        int rent = garut->calculateRent(p2);
        cout << "Stewart mendarat di Garut (milik Kebin), sewa: M" << rent << "\n";
        *p2 -= rent;
        *p1 += rent;
        printPlayerStatus(p1);
        printPlayerStatus(p2);
    }

    // Tax Tile PPH
    cout << "\nStewart mendarat di PPH (flat M150):\n";
    TaxTile* pph = dynamic_cast<TaxTile*>(board->getTileAt(5));
    if (pph != nullptr) {
        before = p2->getMoney();
        pph->collectTax(p2);
        cout << "Bayar pajak: M" << (before - p2->getMoney()) << "\n";
        printPlayerStatus(p2);
    }

    // FreeParkingTile
    cout << "\nGro mendarat di Bebas Parkir:\n";
    before = p3->getMoney();
    FreeParkingTile* fp = dynamic_cast<FreeParkingTile*>(board->getTileAt(20));
    if (fp) fp->onLand(p3, ctx);
    cout << "Perubahan uang: M" << (p3->getMoney() - before) << " (harusnya 0)\n";

    // GoToJail
    cout << "\nKebin mendarat di Pergi ke Penjara:\n";
    GoToJailTile* gtj = dynamic_cast<GoToJailTile*>(board->getTileAt(30));
    if (gtj) gtj->onLand(p1, ctx);
    printPlayerStatus(p1);

    // ChanceTile
    cout << "\nStewart mendarat di Kesempatan:\n";
    printPlayerStatus(p2);
    ChanceTile* chance = dynamic_cast<ChanceTile*>(board->getTileAt(7));
    if (chance) chance->onLand(p2, ctx);
    cout << "Setelah kartu kesempatan:\n";
    printPlayerStatus(p2);

    // CommunityChestTile
    cout << "\nGro mendarat di Dana Umum:\n";
    printPlayerStatus(p3);
    CommunityChestTile* comm = dynamic_cast<CommunityChestTile*>(board->getTileAt(3));
    if (comm) comm->onLand(p3, ctx);
    cout << "Setelah kartu dana umum:\n";
    for (Player* p : allPlayers) printPlayerStatus(p);

    // ══════════════════════════════════════════════════════
    // TEST 8 — Operator Perbandingan Player
    // ══════════════════════════════════════════════════════
    printSeparator("TEST 8: Operator Perbandingan (WinConditionChecker)");

    cout << "Uang saat ini:\n";
    for (Player* p : allPlayers) printPlayerStatus(p);

    cout << "\nKebin > Stewart: " << (*p1 > *p2 ? "YA" : "TIDAK") << "\n";
    cout << "Kebin < Gro: "     << (*p1 < *p3 ? "YA" : "TIDAK") << "\n";

    // ══════════════════════════════════════════════════════
    // Cleanup
    // ══════════════════════════════════════════════════════
    printSeparator("SELESAI");
    cout << "Status akhir semua pemain:\n";
    for (Player* p : allPlayers) printPlayerStatus(p);

    // Cleanup tiles di board
    for (int i = 0; i < 40; i++) {
        delete board->getTileAt(i);
    }
    delete board;
    delete chanceDeck;
    delete communityDeck;
    delete ctx;
    delete p1; delete p2; delete p3;

    return 0;
}