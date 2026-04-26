#include "models/Managers/ManagerTransaksi.hpp"
#include "models/PlayerActionService.hpp"
#include "models/Managers/ManagerProperti.hpp"
#include "models/Pemain.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Papan.hpp"
#include "utils/LogTransaksiGame.hpp"
#include "utils/NimonspoliException.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

PlayerActionService::PlayerActionService( MovementController* mc, ManagerPenjara* mp, ManagerProperti* mpr, ManagerFestival* mf, ManagerTransaksi* mt, Papan* papan, LogTransaksiGame* log, std::vector<Pemain*>* pemain, int* turnIdx) : movementController(mc), managerPenjara(mp), managerProperti(mpr), managerFestival(mf), managerTransaksi(mt), papan(papan), logger(log), daftarPemain(pemain), arahNormal(true), currentTurnIdx(turnIdx) {}

void PlayerActionService::logAksi(Pemain& pemain, const std::string& aksi, const std::string& detail) {
    if (logger) {
        logger->tambahLog(LogTransaksiEntry(0, pemain.getUsername(), aksi, detail));
    }
}

void PlayerActionService::logFestivalActivation(Pemain& pemain, PetakProperti& prop, int pengali, int durasi) {
    std::string detail = prop.getKode() + ": sewa x" + std::to_string(pengali) + ", durasi " + std::to_string(durasi) + " giliran";
    logAksi(pemain, "FESTIVAL", detail);
}

void PlayerActionService::transferMoney(Pemain* asal, Pemain* tujuan, int jumlah) {
    managerTransaksi->transferMoney(asal, tujuan, jumlah);
}

void PlayerActionService::collectFromAllPlayers(Pemain& penerima, int jumlahPerOrang) {
    managerTransaksi->collectFromAllPlayers(penerima, jumlahPerOrang);
}

void PlayerActionService::payToAllPlayers(Pemain& pembayar, int jumlahPerOrang) {
    managerTransaksi->payToAllPlayers(pembayar, jumlahPerOrang);
}

void PlayerActionService::movePlayerRelative(Pemain& p, int n) {
    // stub: MovementController::moveSteps() akan dipakai saat integrasi
    int totalPetak = papan ? papan->getTotalPetak() : 40;
    int posiBaru = ((p.getPosisi() + n) % totalPetak + totalPetak) % totalPetak;
    p.setPosisi(posiBaru);
    std::cout << p.getUsername() << " bergerak ke petak " << posiBaru << ".\n";
    logAksi(p, "GERAK", "pindah ke indeks " + std::to_string(posiBaru));
}

void PlayerActionService::teleportByInput(Pemain& p, const std::string& kodeOrIndeks) {
    // stub: validasi kode petak di Papan akan dilakukan saat integrasi
    std::cout << p.getUsername() << " teleport ke \"" << kodeOrIndeks << "\".\n";
    logAksi(p, "TELEPORT", kodeOrIndeks);
}

void PlayerActionService::moveToNearestStation(Pemain& p) {
    // stub: MovementController::findNearestStation() saat integrasi
    std::cout << p.getUsername() << " bergerak ke stasiun terdekat.\n";
    logAksi(p, "GERAK", "stasiun terdekat");
}

void PlayerActionService::sendToJail(Pemain& p) {
    // stub: ManagerPenjara::masukkanKePenjara() saat integrasi
    p.setStatus(StatusPemain::JAILED);
    p.resetPercobaanPenjara();
    std::cout << p.getUsername() << " masuk penjara!\n";
    logAksi(p, "PENJARA", "masuk penjara");
}

void PlayerActionService::escapeFromJailByCard(Pemain& p) {
    p.setStatus(StatusPemain::ACTIVE);
    p.resetPercobaanPenjara();
    std::cout << p.getUsername() << " bebas dari penjara (kartu).\n";
    logAksi(p, "PENJARA", "bebas via kartu");
}

void PlayerActionService::jailOpponent(Pemain& pemain) {
    std::cout << "Pilih pemain yang ingin dipenjara:\n";
    std::vector<Pemain*> aktif;
    for (Pemain* p : *daftarPemain) {
        if (p != &pemain && p->getStatus() == StatusPemain::ACTIVE) aktif.push_back(p);
    }
    if (aktif.empty()) {
        std::cout << "Tidak ada pemain lain yang aktif.\n";
        return;
    }
    for (size_t i = 0; i < aktif.size(); ++i)
        std::cout << "  " << (i+1) << ". " << aktif[i]->getUsername() << "\n";
    int pilihan = 0;
    while (pilihan < 1 || pilihan > static_cast<int>(aktif.size())) {
        std::cout << "Pilih (1-" << aktif.size() << "): ";
        std::cin >> pilihan;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    sendToJail(*aktif[pilihan - 1]);
    logAksi(pemain, "KARTU", "PenjaraKanCard " + aktif[pilihan-1]->getUsername());
}

void PlayerActionService::bayarSewa(Pemain& penyewa, PetakProperti& properti, int nilaiDadu) {
    (void)nilaiDadu;
}
void PlayerActionService::beliProperti(Pemain& pemain, PetakProperti& properti) {
    (void)pemain; (void)properti;
}
void PlayerActionService::gadaiProperti(Pemain& pemain, PetakProperti& properti) {
    (void)pemain; (void)properti;
}
void PlayerActionService::bangunProperti(Pemain& pemain, PetakLahan& properti) {
    (void)pemain; (void)properti;
}

void PlayerActionService::demolishOpponentProperty(Pemain& pemain) {
    std::vector<Pemain*> aktif;
    for (Pemain* p : *daftarPemain) {
        if (p != &pemain && p->getStatus() == StatusPemain::ACTIVE) aktif.push_back(p);
    }
    if (aktif.empty()) {
        std::cout << "Tidak ada pemain lain yang aktif.\n";
        return;
    }

    std::cout << "Pilih pemain yang propertinya ingin dihancurkan:\n";
    for (size_t i = 0; i < aktif.size(); ++i)
        std::cout << "  " << (i+1) << ". " << aktif[i]->getUsername() << "\n";
    int pilihanPemain = 0;
    while (pilihanPemain < 1 || pilihanPemain > static_cast<int>(aktif.size())) {
        std::cout << "Pilih (1-" << aktif.size() << "): ";
        std::cin >> pilihanPemain;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    Pemain* target = aktif[pilihanPemain - 1];

    std::vector<PetakLahan*> punyaBangunan = managerProperti->getPropertiBisaDihancurkan(target);
    
    if (punyaBangunan.empty()) {
        std::cout << target->getUsername() << " tidak punya properti Lahan dengan bangunan\n";
        return;
    }

    std::cout << "Pilih properti yang bangunannya ingin dihancurkan:\n";
    for (size_t i = 0; i < punyaBangunan.size(); ++i) {
        PetakLahan* s = punyaBangunan[i];
        std::cout << "  " << (i+1) << ". " << s->getNama() << " (" << s->getKode() << ")" << " — " << (s->punyaHotel() ? "Hotel" : std::to_string(s->getJumlahBangunan()) + " rumah") << "\n";
    }

    int pilihanProperti = 0;
    while (pilihanProperti < 1 || pilihanProperti > static_cast<int>(punyaBangunan.size())) {
        std::cout << "Pilih (1-" << punyaBangunan.size() << "): ";
        std::cin >> pilihanProperti;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    PetakLahan* dipilih = punyaBangunan[pilihanProperti - 1];

    int jumlahSebelum = managerProperti->hancurkanSemuaBangunan(dipilih);

    std::cout << "[DemolitionCard] Semua bangunan (" << jumlahSebelum << (jumlahSebelum == 5 ? " — hotel" : " rumah") << ") di " << dipilih->getNama() << " milik " << target->getUsername() << " telah dihancurkan.\n";
    logAksi(pemain, "KARTU", "DemolitionCard " + dipilih->getKode() + " milik " + target->getUsername());
}

void PlayerActionService::pullPlayerAhead(Pemain& pemain) {
    int posiSaya = pemain.getPosisi();
    int TOTAL_PETAK = papan ? papan->getTotalPetak() : 40;
    std::vector<Pemain*> kandidat;
    for (Pemain* p : *daftarPemain) {
        if (p == &pemain) continue;
        if (p->getStatus() != StatusPemain::ACTIVE) continue; // skip JAILED
        int jarak = (p->getPosisi() - posiSaya + TOTAL_PETAK) % TOTAL_PETAK;
        if (jarak > 0) kandidat.push_back(p);
    }
    if (kandidat.empty()) {
        std::cout << "Tidak ada pemain aktif di depan kamu.\n";
        return;
    }

    // urutkan dari yang terdekat
    std::sort(kandidat.begin(), kandidat.end(), [&](Pemain* a, Pemain* b) {
        int ja = (a->getPosisi() - posiSaya + TOTAL_PETAK) % TOTAL_PETAK;
        int jb = (b->getPosisi() - posiSaya + TOTAL_PETAK) % TOTAL_PETAK;
        return ja < jb;
    });
    std::cout << "Pilih pemain yang ingin ditarik ke posisi kamu (petak " << posiSaya << "):\n";
    for (size_t i = 0; i < kandidat.size(); ++i) {
        int jarak = (kandidat[i]->getPosisi() - posiSaya + TOTAL_PETAK) % TOTAL_PETAK;
        std::cout << "  " << (i+1) << ". " << kandidat[i]->getUsername() << " (petak " << kandidat[i]->getPosisi() << ", jarak " << jarak << ")\n";
    }
    int pilihan = 0;
    while (pilihan < 1 || pilihan > static_cast<int>(kandidat.size())) {
        std::cout << "Pilih (1-" << kandidat.size() << "): ";
        std::cin >> pilihan;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    Pemain* target = kandidat[pilihan - 1];
    std::cout << target->getUsername() << " ditarik ke posisi " << pemain.getUsername() << " (petak " << posiSaya << ").\n";
    target->setPosisi(posiSaya);
    // TODO (integrasi): papan->getPetak(posiSaya)->onLanded(*target, *this)
    logAksi(pemain, "KARTU", "LassoCard : tarik " + target->getUsername() + " ke petak " + std::to_string(posiSaya));
}

void PlayerActionService::rotateAllHandCards(Pemain& pemain) {
    std::vector<Pemain*> aktif;
    for (Pemain* p : *daftarPemain) {
        if (p->getStatus() == StatusPemain::ACTIVE) aktif.push_back(p);
    }
    if (aktif.size() < 2) {
        std::cout << "Tidak cukup pemain untuk rotasi kartu.\n";
        return;
    }
    // simpan hand pemain terakhir sebagai buffer
    std::vector<KartuKemampuanSpesial*> buffer = aktif.back()->getKartuDiTangan();
    // proses rotasi: hand[i] mendapat hand[i-1], dari belakang ke depan
    for (int i = static_cast<int>(aktif.size()) - 1; i > 0; --i) {
        aktif[i]->setKartuDiTangan(aktif[i - 1]->getKartuDiTangan());
    }
    aktif[0]->setKartuDiTangan(buffer);
    std::cout << "[RotasiKartuCard] Kartu tangan semua pemain telah dirotasi.\n";
    logAksi(pemain, "KARTU", "RotasiKartuCard — rotasi " + std::to_string(aktif.size()) + " pemain");
}

void PlayerActionService::reverseTurnOrder(Pemain& pemain) {
    arahNormal = !arahNormal;
    std::cout << "[ReverseCard] Urutan giliran kini " << (arahNormal ? "normal (searah jarum jam).\n" : "terbalik!\n");
    logAksi(pemain, "KARTU", arahNormal ? "ReverseCard (normal)" : "ReverseCard (terbalik)");
}

void PlayerActionService::beriSemuaAset (Pemain* asal, Pemain *tujuan) {
    managerTransaksi->beriSemuaAset(asal, tujuan);
}
