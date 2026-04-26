#include "../../include/core/GameEngine.hpp"
#include "../../include/utils/NimonspoliException.hpp"
#include "../../include/views/InputHandler.hpp"
#include "../../include/utils/SaveNLoad.hpp"

GameEngine::GameEngine() 
    : configData(nullptr), papanPermainan(nullptr), logGame(nullptr), deckKartu(nullptr), 
      deckSkill(nullptr), dadu(nullptr), managerPenjara(nullptr), managerProperti(nullptr),
      actionService(nullptr), currentTurnIndex(0), maxTurn(0), arahNormalTurn(true) {
}

GameEngine::~GameEngine() {
    // Cleanup jika diperlukan
    if (configData) delete configData;
    for (Pemain* p : listPemain) {
        delete p;
    }
    // urutanPemain berisi pointer yang sama dengan listPemain (bukan owner)
    urutanPemain.clear();
}

void GameEngine::startGame() {
    // Implementasi logic game utama

    // acak urutan playernya
    randomizeTurn();

    // iterasi sebanyak jumlah turn
    for (currentGlobalTurn = 1; currentGlobalTurn <= maxTurn; currentGlobalTurn++) {

        // jalanin turn buat tiap player
        for (auto player : urutanPemain) {

            if (player->getStatus() == StatusPemain::BANKRUPT) { // yang bangkrut di skip
                continue;
            }

            if (SelesaiGame::sisaSatuOrang(listPemain)) { // kalo di tengah2 turn udah tinggal sisa satu orang, game langsung selesai
                SelesaiGame::tampilPanelSelesaiKarenaBangkrut();
                SelesaiGame::tampilkanPemenang (player);
                return;
            }


            // player bisa pilih mau liat2 akta, atau liat2 properti dia sendiri, ato mau jalan, ato mau make kartu
            startTurn (player, currentGlobalTurn);

            // habis turnya selesai normalnya player pindah tempat -> jalanin onlanded dari petak yang diinjek
            Petak* diTempati = papanPermainan->getPetak((const int)player->getPosisi());
            diTempati->onLanded(*player, *actionService);
            
        }
    }

    Pemain* pemenang = SelesaiGame::getPemenang (listPemain);
    SelesaiGame::tampilPanelSelesaiKarenaMaxTurn(maxTurn);
    SelesaiGame::tampilkanPemenang (pemenang);

}

void GameEngine::randomizeTurn() {
    urutanPemain = listPemain; // pointer asli, state tetap konsisten
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(urutanPemain.begin(), urutanPemain.end(), gen);
}

void GameEngine::startTurn(Pemain* p, int currentTurn) {
    // Tahap 2: command dispatcher + validasi dasar (skill sebelum dadu)
    bool awalGiliran = true;
    bool sudahLemparDadu = false;

    // Awal giliran: reset status dan bagikan 1 kartu skill acak
    p->resetSkillUsage();

    if (deckSkill) {
        try {
            KartuKemampuanSpesial* baru = deckSkill->ambilKartu();
            try {
                p->tambahKartu(baru, *deckSkill);
                OutputHandler::cetakEfekKartu("Kartu Skill", "Kamu mendapatkan: " + baru->getDeskripsi());
            } catch (const SlotKartuPenuhException&) {
                // slot penuh → wajib buang 1 kartu, lalu masukkan kartu baru
                OutputHandler::cetakError("Kamu sudah memiliki 3 kartu (maksimal). Kamu wajib membuang 1 kartu.");
                const auto& hand = p->getKartuDiTangan();
                for (size_t i = 0; i < hand.size(); ++i) {
                    OutputHandler::cetakPesan(std::to_string(i + 1) + ". " + hand[i]->getDeskripsi());
                }
                OutputHandler::cetakPesan(std::to_string(hand.size() + 1) + ". " + baru->getDeskripsi() + " (kartu baru)");
                int pilih = InputHandler::promptAngka("Pilih kartu yang dibuang (1-" + std::to_string(hand.size() + 1) + "): ", 1, hand.size() + 1);
                if (pilih == static_cast<int>(hand.size() + 1)) {
                    // buang kartu baru
                    deckSkill->buangKartu(baru);
                } else {
                    p->buangKartu(pilih - 1, *deckSkill);
                    p->tambahKartu(baru, *deckSkill);
                    OutputHandler::cetakEfekKartu("Kartu Skill", "Kamu mendapatkan: " + baru->getDeskripsi());
                }
            }
        } catch (const std::exception&) {
            // deck kosong → abaikan (akan dihandle DeckKartu::acakUlang)
        }
    }

    while (true) {
        try {
            std::string line = InputHandler::promptLine("Perintah> ");
            if (line.empty()) continue;

            // normalisasi sederhana: trim + uppercase + ubah '-' jadi '_' + collapse spasi
            auto trim = [](std::string s) {
                size_t a = s.find_first_not_of(" \t\r\n");
                size_t b = s.find_last_not_of(" \t\r\n");
                if (a == std::string::npos) return std::string();
                return s.substr(a, b - a + 1);
            };
            line = trim(line);
            for (char& c : line) {
                if (c == '-') c = '_';
                c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            }

            // tokenisasi by space
            std::vector<std::string> tok;
            {
                std::string cur;
                for (char c : line) {
                    if (c == ' ' || c == '\t') {
                        if (!cur.empty()) { tok.push_back(cur); cur.clear(); }
                    } else cur.push_back(c);
                }
                if (!cur.empty()) tok.push_back(cur);
            }
            if (tok.empty()) continue;
            const std::string& cmd = tok[0];

            if (cmd == "CETAK_PAPAN") {
                OutputHandler::cetakPapan(*papanPermainan, listPemain, currentTurn, maxTurn);
                awalGiliran = false;
                continue;
            }

            if (cmd == "CETAK_PROPERTI") {
                if (configData) OutputHandler::cetakPropertiPemain(p, *configData);
                awalGiliran = false;
                continue;
            }

            if (cmd == "CETAK_LOG") {
                int top = -1;
                if (tok.size() >= 2) top = std::stoi(tok[1]);
                if (logGame) {
                    if (top > 0) {
                        auto logs = logGame->getLogs(top);
                        OutputHandler::cetakPesan("=== Log Transaksi (" + std::to_string(top) + " Terakhir) ===");
                        for (const auto& e : logs) {
                            OutputHandler::cetakPesan("[Turn " + std::to_string(e.getRonde()) + "] " + e.getUsername() + " | " + e.getAksi() + " | " + e.getDetail());
                        }
                    } else {
                        OutputHandler::cetakPesan("=== Log Transaksi Penuh ===");
                        for (const auto& e : logGame->getLogs()) {
                            OutputHandler::cetakPesan("[Turn " + std::to_string(e.getRonde()) + "] " + e.getUsername() + " | " + e.getAksi() + " | " + e.getDetail());
                        }
                    }
                }
                awalGiliran = false;
                continue;
            }

            if (cmd == "GUNAKAN_KEMAMPUAN") {
                if (sudahLemparDadu) {
                    OutputHandler::cetakError("Kartu kemampuan hanya bisa digunakan SEBELUM melempar dadu.");
                    continue;
                }
                const auto& hand = p->getKartuDiTangan();
                if (hand.empty()) {
                    OutputHandler::cetakError("Kamu tidak punya kartu kemampuan.");
                    continue;
                }
                OutputHandler::cetakPesan("Daftar Kartu Kemampuan Spesial Anda:");
                for (size_t i = 0; i < hand.size(); ++i) {
                    OutputHandler::cetakPesan(std::to_string(i + 1) + ". " + hand[i]->getDeskripsi());
                }
                int pilih = InputHandler::promptAngka("Pilih kartu (1-" + std::to_string(hand.size()) + ", 0 batal): ", 0, hand.size());
                if (pilih == 0) continue;
                p->gunakanKartu(pilih - 1, *actionService, *deckSkill, true);
                awalGiliran = false;
                continue;
            }

            if (cmd == "LEMPAR_DADU") {
                if (sudahLemparDadu) {
                    OutputHandler::cetakError("Kamu sudah melempar dadu di giliran ini.");
                    continue;
                }
                if (p->getStatus() == StatusPemain::ACTIVE) {
                    dadu->rollRandom();
                    OutputHandler::cetakPesan("Hasil: " + std::to_string(dadu->getNilaid1()) + " + " + std::to_string(dadu->getNilaid2()) + " = " + std::to_string(dadu->getTotalNilaiDadu()));
                    actionService->movePlayerRelative(*p, dadu->getTotalNilaiDadu());
                    sudahLemparDadu = true;
                    break; // selesai phase command → lanjut onLanded
                }
                OutputHandler::cetakError("Kamu tidak bisa melempar dadu saat status tidak ACTIVE.");
                continue;
            }

            if (cmd == "ATUR_DADU") {
                if (sudahLemparDadu) {
                    OutputHandler::cetakError("Kamu sudah melempar dadu di giliran ini.");
                    continue;
                }
                if (tok.size() < 3) {
                    OutputHandler::cetakError("Format: ATUR_DADU X Y");
                    continue;
                }
                int x = std::stoi(tok[1]);
                int y = std::stoi(tok[2]);
                if (x < 1 || x > 6 || y < 1 || y > 6) {
                    OutputHandler::cetakError("Nilai dadu harus 1-6.");
                    continue;
                }
                if (p->getStatus() != StatusPemain::ACTIVE) {
                    OutputHandler::cetakError("Tidak bisa ATUR_DADU saat kamu dipenjara.");
                    continue;
                }
                dadu->rollManual(x, y);
                OutputHandler::cetakPesan("Hasil: " + std::to_string(x) + " + " + std::to_string(y) + " = " + std::to_string(dadu->getTotalNilaiDadu()));
                actionService->movePlayerRelative(*p, dadu->getTotalNilaiDadu());
                sudahLemparDadu = true;
                break;
            }

            if (cmd == "SIMPAN") {
                if (!awalGiliran) {
                    OutputHandler::cetakError("Tidak bisa menyimpan game di tengah-tengah giliran!");
                    continue;
                }
                if (tok.size() < 2) {
                    OutputHandler::cetakError("Format: SIMPAN <filename>");
                    continue;
                }
                SaveNLoad snl;
                snl.saveGameState(*this, tok[1]);
                OutputHandler::cetakPesan("Permainan berhasil disimpan ke: " + tok[1]);
                continue;
            }

            if (cmd == "BANTUAN" || cmd == "HELP") {
                OutputHandler::cetakPesan("Perintah tersedia: CETAK_PAPAN, CETAK_PROPERTI, CETAK_LOG [N], GUNAKAN_KEMAMPUAN, LEMPAR_DADU, ATUR_DADU X Y, SIMPAN <file>");
                continue;
            }

            throw PerintahTidakDitemukanException();
        } catch (const NimonspoliException& e) {
            OutputHandler::cetakError(e.what());
            OutputHandler::cetakPesan("Silakan coba perintah lain.");
        } catch (const std::exception& e) {
            OutputHandler::cetakError(e.what());
        }
    }
}
