#include "utils/SaveNLoad.hpp"
#include <sstream>
#include <fstream>
#include "utils/FileHelper.hpp"
#include "utils/NimonspoliException.hpp"
#include "core/GameEngine.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Petak/PetakLahan.hpp"
#include "models/Petak/Petak.hpp"
#include "models/Kartu/KartuKemampuanSpesial.hpp"
#include "models/Kartu/DeckKartu.hpp"
#include "models/Managers/ManagerFestival.hpp"

using namespace std;

void SaveNLoad::loadGameState(GameEngine& game, string filename) {
    // To be implemented in Phase 5C
}

void SaveNLoad::saveGameState(GameEngine& game, string filename) {
    std::vector<std::string> lines;

    // <TURN_SAAT_INI> <MAX_TURN>
    // Asumsi GameEngine punya getCurrentGlobalTurn(), untuk sementara pakai 1 jika belum ada
    int currentTurn = game.getCurrentGlobalTurn(); 
    int maxTurn = game.getMaxTurn();
    lines.push_back(to_string(currentTurn) + " " + to_string(maxTurn));

    // <JUMLAH_PEMAIN>
    auto& listPemain = game.getListPemain();
    lines.push_back(to_string(listPemain.size()));

    // <STATE_PEMAIN_1> ...
    for (Pemain* p : listPemain) {
        // <USERNAME> <UANG> <POSISI_PETAK> <STATUS>
        string statusStr = "ACTIVE";
        if (p->getStatus() == StatusPemain::BANKRUPT) statusStr = "BANKRUPT";
        else if (p->getStatus() == StatusPemain::JAILED) statusStr = "JAILED";

        string kodePetak = "START";
        if (game.getPapanPermainan()) {
            Petak* petak = game.getPapanPermainan()->getPetak(p->getPosisi());
            if (petak) kodePetak = petak->getKode();
        }

        lines.push_back(p->getUsername() + " " + to_string(p->getSaldo()) + " " + kodePetak + " " + statusStr);

        // <JUMLAH_KARTU_TANGAN>
        auto& kartuTangan = p->getKartuDiTangan();
        lines.push_back(to_string(kartuTangan.size()));

        // <JENIS_KARTU> <NILAI_KARTU>
        for (auto* kartu : kartuTangan) {
            string namaKartu = kartu->getNamaKartu();
            string nilaiKartu = "";
            
            // Format spesifik nilai kartu jika ada
            if (namaKartu == "MoveCard") {
                // Asumsi bisa mendapatkan langkah dari kartu, untuk stub pakai 1
                nilaiKartu = " 1"; // Harus diakses dari getter jika ada
            } else if (namaKartu == "DiscountCard") {
                nilaiKartu = " 30"; // Stub
            }
            // Karena spesifikasi meminta nilai dikosongkan jika tidak ada, 
            // formatnya adalah string kosong jika tidak ada nilai.
            lines.push_back(namaKartu + nilaiKartu);
        }
    }

    // <URUTAN_GILIRAN_1> ...
    auto& urutanPemain = game.getUrutanPemain();
    string urutanStr = "";
    for (size_t i = 0; i < urutanPemain.size(); ++i) {
        urutanStr += urutanPemain[i]->getUsername() + (i == urutanPemain.size() - 1 ? "" : " ");
    }
    lines.push_back(urutanStr);

    // <GILIRAN_AKTIF_SAAT_INI>
    int curIdx = game.getCurrentTurnIndex();
    if (curIdx >= 0 && curIdx < static_cast<int>(urutanPemain.size())) {
        lines.push_back(urutanPemain[curIdx]->getUsername());
    } else {
        lines.push_back(urutanPemain.empty() ? "None" : urutanPemain[0]->getUsername());
    }

    // <STATE_PROPERTI>
    // Asumsi papanPermainan menyimpan list semua petak. Kita hitung jumlah properti dulu
    std::vector<PetakProperti*> semuaProperti;
    if (game.getPapanPermainan()) {
        for (Petak* petak : game.getPapanPermainan()->getDaftarPetak()) {
            if (auto prop = dynamic_cast<PetakProperti*>(petak)) {
                semuaProperti.push_back(prop);
            }
        }
    }
    lines.push_back(to_string(semuaProperti.size()));

    for (PetakProperti* prop : semuaProperti) {
        string kode = prop->getKode();
        string jenis = prop->getJenisString();
        string pemilik = prop->getPemilik() ? prop->getPemilik()->getUsername() : "BANK";
        
        string statusProp = "BANK";
        if (prop->getStatus() == PetakProperti::StatusProperti::OWNED) statusProp = "OWNED";
        else if (prop->getStatus() == PetakProperti::StatusProperti::MORTGAGED) statusProp = "MORTGAGED";

        int fmult = 1;
        int fdur = 0;
        if (game.getManagerFestival()) {
            if (game.getManagerFestival()->isAktif(prop)) {
                fmult = game.getManagerFestival()->getPengali(prop);
                fdur = game.getManagerFestival()->getSisaDurasi(prop);
            }
        }

        string nBangunan = "0";
        if (auto lahan = dynamic_cast<PetakLahan*>(prop)) {
            if (lahan->punyaHotel()) nBangunan = "H";
            else nBangunan = to_string(lahan->getJumlahBangunan());
        }

        // <KODE_PETAK> <JENIS> <PEMILIK> <STATUS> <FMULT> <FDUR> <N_BANGUNAN>
        // Gunakan padding statis seperti contoh atau spasi saja (spesifikasi biasanya bisa parse spasi berapapun)
        lines.push_back(kode + " " + jenis + " " + pemilik + " " + statusProp + " " + to_string(fmult) + " " + to_string(fdur) + " " + nBangunan);
    }

    // <STATE_DECK>
    // Asumsi Deck Skill ada
    if (game.getDeckKartu() && false) { // Skip dulu sementara tidak ada DeckKartuKemampuanSpesial
        // ...
    } else {
        lines.push_back("0");
    }

    // <STATE_LOG>
    if (game.getLogGame()) {
        auto& log = game.getLogGame()->getLogs();
        lines.push_back(to_string(log.size()));
        for (auto& entry : log) {
            lines.push_back(to_string(entry.getRonde()) + " " + entry.getUsername() + " " + entry.getAksi() + " " + entry.getDetail());
        }
    } else {
        lines.push_back("0");
    }

    FileHelper::writeAllLines(filename, lines);
}
