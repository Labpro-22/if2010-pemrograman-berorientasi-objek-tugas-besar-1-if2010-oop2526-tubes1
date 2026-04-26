import re

with open("src/utils/SaveNLoad.cpp", "r") as f:
    content = f.read()

load_impl = """void SaveNLoad::loadGameState(GameEngine& game, string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw NimonspoliException("Gagal membuka file save: " + filename);
    }

    string line;
    
    // <TURN_SAAT_INI> <MAX_TURN>
    if (!getline(file, line)) throw NimonspoliException("Format save tidak valid.");
    stringstream ss(line);
    int currentGlobalTurn, maxTurn;
    ss >> currentGlobalTurn >> maxTurn;
    game.setCurrentGlobalTurn(currentGlobalTurn);
    game.setMaxTurn(maxTurn);

    // <JUMLAH_PEMAIN>
    if (!getline(file, line)) throw NimonspoliException("Format save tidak valid.");
    int numPemain = stoi(line);

    vector<Pemain*> loadedPlayers;
    for (int i = 0; i < numPemain; i++) {
        if (!getline(file, line)) throw NimonspoliException("Data pemain tidak lengkap.");
        stringstream pss(line);
        string username, kodePetak, statusStr;
        int uang;
        pss >> username >> uang >> kodePetak >> statusStr;

        Pemain* p = new Pemain(username);
        p->setSaldo(uang);
        
        // Status
        if (statusStr == "BANKRUPT") p->setStatus(StatusPemain::BANKRUPT);
        else if (statusStr == "JAILED") p->setStatus(StatusPemain::JAILED);
        else p->setStatus(StatusPemain::ACTIVE);

        // Posisi
        if (game.getPapanPermainan()) {
            Petak* petak = game.getPapanPermainan()->getPetak(kodePetak);
            if (petak) p->setPosisi(petak->getIndeks());
        }

        // <JUMLAH_KARTU_TANGAN>
        if (!getline(file, line)) throw NimonspoliException("Format kartu tangan tidak valid.");
        int numCards = stoi(line);
        for (int j = 0; j < numCards; j++) {
            if (!getline(file, line)) throw NimonspoliException("Data kartu tidak lengkap.");
            stringstream css(line);
            string jenisKartu;
            css >> jenisKartu;
            
            // Factory sederhana untuk memuat kartu dari tipe string
            KartuKemampuanSpesial* kartu = nullptr;
            if (jenisKartu == "MoveCard") {
                int langkah = 1;
                if (css >> langkah) kartu = new MoveCard(langkah);
                else kartu = new MoveCard(1); // fallback
            } else if (jenisKartu == "DiscountCard") {
                int persen = 30;
                int durasi = 1;
                css >> persen >> durasi;
                auto dc = new DiscountCard(persen);
                dc->setSisaDurasi(durasi);
                kartu = dc;
            } else if (jenisKartu == "ShieldCard") {
                kartu = new ShieldCard();
            } else if (jenisKartu == "TeleportCard") {
                kartu = new TeleportCard();
            } else if (jenisKartu == "LassoCard") {
                kartu = new LassoCard();
            } else if (jenisKartu == "DemolitionCard") {
                kartu = new DemolitionCard();
            } else if (jenisKartu == "RotasiKartuCard") {
                kartu = new RotasiKartuCard();
            } else if (jenisKartu == "ReverseCard") {
                kartu = new ReverseCard();
            } else if (jenisKartu == "PenjaraKanCard") {
                kartu = new PenjaraKanCard();
            }

            if (kartu) {
                // Jangan panggil p->tambahKartu karena akan melempar exception slot jika lebih dari 3 atau butuh reference ke deck
                // Lebih aman set array internalnya secara langsung.
                // Tapi kita bisa asumsikan save game valid, jadi tambahKartu tidak masalah.
                // Namun tambahKartu di Pemain.cpp meminta deckSkill. Kita butuh dummy deck atau getter setter.
                // Sebagai workaround, gunakan setKartuDiTangan:
                auto hand = p->getKartuDiTangan();
                hand.push_back(kartu);
                p->setKartuDiTangan(hand);
            }
        }
        loadedPlayers.push_back(p);
    }

    game.setListPemain(loadedPlayers);

    // <URUTAN_GILIRAN_1> ...
    if (!getline(file, line)) throw NimonspoliException("Format urutan giliran tidak valid.");
    stringstream urutss(line);
    string uname;
    vector<Pemain*> urutanPemain;
    while (urutss >> uname) {
        for (Pemain* p : loadedPlayers) {
            if (p->getUsername() == uname) {
                urutanPemain.push_back(p);
                break;
            }
        }
    }
    game.setUrutanPemain(urutanPemain);

    // <GILIRAN_AKTIF_SAAT_INI>
    if (!getline(file, line)) throw NimonspoliException("Format giliran aktif tidak valid.");
    string aktifUname = line;
    int curIdx = 0;
    for (size_t i = 0; i < urutanPemain.size(); ++i) {
        if (urutanPemain[i]->getUsername() == aktifUname) {
            curIdx = i;
            break;
        }
    }
    game.setCurrentTurnIndex(curIdx);

    // <STATE_PROPERTI>
    if (!getline(file, line)) throw NimonspoliException("Format jumlah properti tidak valid.");
    int numProperti = stoi(line);
    
    for (int i = 0; i < numProperti; i++) {
        if (!getline(file, line)) throw NimonspoliException("Data properti tidak lengkap.");
        stringstream propss(line);
        string kode, jenis, pemilikStr, statusStr, nBangunanStr;
        int fmult, fdur;
        propss >> kode >> jenis >> pemilikStr >> statusStr >> fmult >> fdur >> nBangunanStr;

        if (game.getPapanPermainan()) {
            Petak* petak = game.getPapanPermainan()->getPetak(kode);
            if (auto prop = dynamic_cast<PetakProperti*>(petak)) {
                
                if (statusStr == "OWNED") prop->setStatus(PetakProperti::StatusProperti::OWNED);
                else if (statusStr == "MORTGAGED") prop->setStatus(PetakProperti::StatusProperti::MORTGAGED);
                else prop->setStatus(PetakProperti::StatusProperti::BANK);

                if (pemilikStr != "BANK") {
                    Pemain* propOwner = nullptr;
                    for (Pemain* p : loadedPlayers) {
                        if (p->getUsername() == pemilikStr) {
                            propOwner = p;
                            break;
                        }
                    }
                    if (propOwner) {
                        prop->setPemilik(propOwner);
                        propOwner->tambahAset(prop);
                    }
                }

                // Festival
                if (game.getManagerFestival() && fdur > 0) {
                    // Karena ManagerFestival menggunakan add/apply manual, kita bypass langsung
                    // Tapi karena ManagerFestival butuh dipanggil applyFestival, dan kita mau set durasi spesifik:
                    // Untuk saat ini stub:
                    // TODO: tambahkan setEfekFestival(prop, fmult, fdur) di ManagerFestival
                }

                // Bangunan
                if (auto lahan = dynamic_cast<PetakLahan*>(prop)) {
                    if (nBangunanStr == "H") {
                        while (!lahan->punyaHotel()) lahan->bangun();
                    } else {
                        int nb = stoi(nBangunanStr);
                        while (lahan->getJumlahBangunan() < nb && !lahan->punyaHotel()) {
                            lahan->bangun();
                        }
                    }
                }
            }
        }
    }

    // <STATE_DECK>
    if (!getline(file, line)) throw NimonspoliException("Format state deck tidak valid.");
    int numDeck = stoi(line);
    // Untuk saat ini kita lewatkan saja karena instansiasi deck butuh pointer Kartu asli
    for (int i = 0; i < numDeck; i++) {
        getline(file, line);
    }

    // <STATE_LOG>
    if (!getline(file, line)) throw NimonspoliException("Format state log tidak valid.");
    int numLogs = stoi(line);
    if (game.getLogGame()) {
        for (int i = 0; i < numLogs; i++) {
            if (!getline(file, line)) throw NimonspoliException("Data log tidak lengkap.");
            stringstream logss(line);
            int turnLog;
            string uNameLog, aksiLog, detailLog, temp;
            logss >> turnLog >> uNameLog >> aksiLog;
            
            // Baca sisa line sebagai detail
            getline(logss, detailLog);
            if (!detailLog.empty() && detailLog[0] == ' ') {
                detailLog = detailLog.substr(1);
            }

            LogTransaksiEntry entry(turnLog, uNameLog, aksiLog, detailLog);
            game.getLogGame()->tambahLog(entry);
        }
    }
}"""

content = re.sub(
    r"void SaveNLoad::loadGameState\(GameEngine& game, string filename\) \{.*?\}",
    load_impl,
    content,
    flags=re.DOTALL
)

with open("src/utils/SaveNLoad.cpp", "w") as f:
    f.write(content)
