// #include "views/BoardRenderer.hpp"
// #include <iomanip>
// #include <iostream>
// #include <sstream>

// // Lebar sel: 10 karakter konten + 2 border = 12 total per sel
// // 9 sel di atas/bawah = 9*10 + 10 border chars = 100 karakter total
// // Area tengah = total lebar - 2 sel samping
// // Total lebar board = 9 * 10 + 10 = 100 karakter
// // Sel samping: "| XXXXXXXX " = 12 karakter
// // Area tengah: 100 - 12 - 12 = 76 karakter (isi saja, tanpa border luar)
// // Format baris tengah: "| CODE     " + [76 char tengah] + "| CODE     |"
// //                       12 char       76 char              12 char
// // Total = 12 + 76 + 12 = 100 ✓

// static const int CELL_W   = 10;  // lebar konten sel
// static const int N_CELLS  = 9;   // jumlah sel atas/bawah
// static const int MID_W    = 76;  // lebar area tengah (tanpa border kiri/kanan sel samping)

// // Helper: cetak garis border penuh (9 sel)
// void BoardRenderer::printBorder(int cells) {
//     for (int i = 0; i < cells; i++) std::cout << "+----------";
//     std::cout << "+" << std::endl;
// }

// // Helper: isi area tengah dengan padding ke kanan agar selalu MID_W karakter
// static void printMiddleArea(int row, int currTurn) {
//     std::string content;
//     switch (row) {
//         case 0: content = "  =================================="; break;
//         case 1: content = "  ||          NIMONSPOLI           ||"; break;
//         case 2: content = "  =================================="; break;
//         case 3: content = "            TURN " + std::to_string(currTurn) + " / 50"; break;
//         case 4: content = "  ----------------------------------"; break;
//         case 5: content = "  LEGENDA KEPEMILIKAN & STATUS"; break;
//         case 6: content = "  P1-P4 : Properti milik Pemain 1-4"; break;
//         case 7: content = "  ^     : Rumah Level 1"; break;
//         case 8: content = "  ^^    : Rumah Level 2"; break;
//         case 9: content = "  ^^^   : Rumah Level 3"; break;
//         case 10: content = "  *     : Hotel (Maksimal)"; break;
//         case 11: content = "  (1)-(4): Bidak (IN=Tahanan, V=Mampir)"; break;
//         case 12: content = "  ----------------------------------"; break;
//         case 13: content = "  KODE WARNA:"; break;
//         case 14: content = "  [CK]=Coklat   [MR]=Merah"; break;
//         case 15: content = "  [BM]=Biru Muda [KN]=Kuning"; break;
//         case 16: content = "  [PK]=Pink      [HJ]=Hijau"; break;
//         case 17: content = "  [OR]=Orange    [BT]=Biru Tua"; break;
//         case 18: content = "  [DF]=Aksi      [AB]=Utilitas"; break;
//         default: content = ""; break;
//     }
//     // Selalu cetak tepat MID_W karakter (padding dengan spasi)
//     std::ostringstream oss;
//     oss << std::left << std::setw(MID_W) << content;
//     std::cout << oss.str();
// }

// // Helper: border baris tengah (sisi kiri + kosong MID_W + sisi kanan)
// static void printMidBorder() {
//     std::cout << "+----------+";
//     for (int i = 0; i < MID_W; i++) std::cout << ' ';
//     std::cout << "+----------+" << std::endl;
// }

// void BoardRenderer::renderBoard(const std::vector<std::string>& tileCodes, int currTurn) {

//     // ── 1. BLOK ATAS (Petak 16–24, kiri ke kanan) ────────────────────────────
//     printBorder(N_CELLS);

//     // Baris nama petak
//     for (int i = 16; i <= 24; i++) {
//         std::cout << "| " << RED << std::left << std::setw(8) << tileCodes[i] << RESET;
//     }
//     std::cout << "|" << std::endl;

//     // Baris info (dummy level/bidak)
//     for (int i = 16; i <= 24; i++) std::cout << "|    ^^    ";
//     std::cout << "|" << std::endl;

//     printBorder(N_CELLS);

//     // ── 2. BLOK TENGAH (Kiri 25–31, Kanan 15–9) ──────────────────────────────
//     // 7 baris, tiap baris: sel kiri | area tengah | sel kanan
//     for (int i = 0; i < 7; i++) {
//         int idxKiri  = 25 + i;   // 25,26,27,28,29,30,31
//         int idxKanan = 15 - i;   // 15,14,13,12,11,10, 9

//         // -- Baris 1: kode petak --
//         std::cout << "| " << CYAN << std::left << std::setw(8) << tileCodes[idxKiri] << RESET << "|";
//         printMiddleArea(i * 3, currTurn);   // row konten: 0,3,6,9,12,15,18
//         std::cout << "| " << GREEN << std::left << std::setw(8) << tileCodes[idxKanan] << RESET << "|" << std::endl;

//         // -- Baris 2: info bidak/level --
//         std::cout << "|    (1)   |";
//         printMiddleArea(i * 3 + 1, currTurn);
//         std::cout << "|    *     |" << std::endl;

//         // -- Baris 3: info tambahan --
//         std::cout << "|          |";
//         printMiddleArea(i * 3 + 2, currTurn);
//         std::cout << "|          |" << std::endl;

//         // Border antar sel (kecuali setelah baris terakhir)
//         if (i < 6) printMidBorder();
//     }

//     // ── 3. BLOK BAWAH (Petak 0–8, kiri ke kanan) ────────────────────────────
//     printBorder(N_CELLS);

//     for (int i = 0; i <= 8; i++) {
//         std::cout << "| " << CYAN << std::left << std::setw(8) << tileCodes[i] << RESET;
//     }
//     std::cout << "|" << std::endl;

//     for (int i = 0; i <= 8; i++) std::cout << "|          ";
//     std::cout << "|" << std::endl;

//     printBorder(N_CELLS);
// }

// BoardRenderer.cpp
// Render papan Nimonspoli (40 petak, format sesuai spesifikasi)
//
// Layout ukuran:
//   Sel          : "|" + 9 char konten = 10 char
//   Baris penuh  : 11 sel × 10 + "|" = 111 char
//   Border penuh : 11 × "+----------" + "+" = 122 char
//   Baris tengah : kiri(10)+"|"+MID(98)+"|"+kanan(9)+"|" = 120 char
//   Border tengah: "+----------+"(12)+spasi(96)+"+----------+"(12) = 120 char
//
// Urutan petak di papan:
//   Atas  : 21(BBP)..31(PPJ)  — kiri ke kanan
//   Kiri  : 20(SBY)..12(MGL)  — atas ke bawah
//   Kanan : 32(BDG)..40(IKN)  — atas ke bawah
//   Bawah : 11(PEN)..1(GO)    — tampil kiri ke kanan

#include "views/BoardRenderer.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

// ── Konstanta ─────────────────────────────────────────────────────────────────
static const int MID_W         = 98;   // lebar area tengah legenda
static const int NCOLS         = 11;   // jumlah kolom papan

// ── ANSI color codes (sesuai kategori warna spesifikasi) ─────────────────────
// Warna untuk tag [XX] di papan
#define RST     "\033[0m"
#define C_CK    "\033[38;5;130m"   // Coklat
#define C_BM    "\033[38;5;117m"   // Biru Muda
#define C_PK    "\033[38;5;213m"   // Pink
#define C_OR    "\033[38;5;214m"   // Orange
#define C_MR    "\033[31m"         // Merah
#define C_KN    "\033[33m"         // Kuning
#define C_HJ    "\033[32m"         // Hijau
#define C_BT    "\033[38;5;27m"    // Biru Tua
#define C_AB    "\033[38;5;245m"   // Abu-abu (Utilitas)
#define C_DF    "\033[38;5;240m"   // Default (Aksi/Spesial)

// ── Data petak (kode, tag, warna) ────────────────────────────────────────────
struct TileData { std::string code, tag; const char* color; };

static TileData TILES[41] = {
/*  0 */ {},
/*  1 */ {"GO",  "[DF]", C_DF},  // Petak Mulai
/*  2 */ {"GRT", "[CK]", C_CK},  // Garut
/*  3 */ {"DNU", "[DF]", C_DF},  // Dana Umum
/*  4 */ {"TSK", "[CK]", C_CK},  // Tasikmalaya
/*  5 */ {"PPH", "[DF]", C_DF},  // Pajak Penghasilan
/*  6 */ {"GBR", "[DF]", C_DF},  // Stasiun Gambir
/*  7 */ {"BGR", "[BM]", C_BM},  // Bogor
/*  8 */ {"FES", "[DF]", C_DF},  // Festival
/*  9 */ {"DPK", "[BM]", C_BM},  // Depok
/* 10 */ {"BKS", "[BM]", C_BM},  // Bekasi
/* 11 */ {"PEN", "[DF]", C_DF},  // Penjara
/* 12 */ {"MGL", "[PK]", C_PK},  // Magelang
/* 13 */ {"PLN", "[AB]", C_AB},  // PLN (Utilitas)
/* 14 */ {"SOL", "[PK]", C_PK},  // Solo
/* 15 */ {"YOG", "[PK]", C_PK},  // Yogyakarta
/* 16 */ {"STB", "[DF]", C_DF},  // Stasiun Bandung
/* 17 */ {"MAL", "[OR]", C_OR},  // Malang
/* 18 */ {"DNU", "[DF]", C_DF},  // Dana Umum
/* 19 */ {"SMG", "[OR]", C_OR},  // Semarang
/* 20 */ {"SBY", "[OR]", C_OR},  // Surabaya
/* 21 */ {"BBP", "[DF]", C_DF},  // Bebas Parkir
/* 22 */ {"MKS", "[MR]", C_MR},  // Makassar
/* 23 */ {"KSP", "[DF]", C_DF},  // Kesempatan
/* 24 */ {"BLP", "[MR]", C_MR},  // Balikpapan
/* 25 */ {"MND", "[MR]", C_MR},  // Manado
/* 26 */ {"TUG", "[DF]", C_DF},  // Stasiun Tugu
/* 27 */ {"PLB", "[KN]", C_KN},  // Palembang
/* 28 */ {"PKB", "[KN]", C_KN},  // Pekanbaru
/* 29 */ {"PAM", "[AB]", C_AB},  // PAM (Utilitas)
/* 30 */ {"MED", "[KN]", C_KN},  // Medan
/* 31 */ {"PPJ", "[DF]", C_DF},  // Pergi ke Penjara
/* 32 */ {"BDG", "[HJ]", C_HJ},  // Bandung
/* 33 */ {"DEN", "[HJ]", C_HJ},  // Denpasar
/* 34 */ {"FES", "[DF]", C_DF},  // Festival
/* 35 */ {"MTR", "[HJ]", C_HJ},  // Mataram
/* 36 */ {"GUB", "[DF]", C_DF},  // Stasiun Gubeng
/* 37 */ {"KSP", "[DF]", C_DF},  // Kesempatan
/* 38 */ {"JKT", "[BT]", C_BT},  // Jakarta
/* 39 */ {"PBM", "[DF]", C_DF},  // Pajak Barang Mewah
/* 40 */ {"IKN", "[BT]", C_BT},  // Ibu Kota Nusantara
};

// ── Helper cetak sel ──────────────────────────────────────────────────────────

// Baris 1 kiri/tengah: "|[TAG] COD " = 10 char
static void printCell1(int idx) {
    auto& t = TILES[idx];
    std::string c = t.code; c.resize(3, ' ');
    std::cout << "|" << t.color << t.tag << " " << c << RST << " ";
}

// Baris 1 kanan (dengan | penutup): "|[TAG] COD |"  → pasangkan dengan printMid
static void printCell1R(int idx) {
    auto& t = TILES[idx];
    std::string c = t.code; c.resize(3, ' ');
    std::cout << "|" << t.color << t.tag << " " << c << RST << " |";
}

// Baris 2 kiri: "|info     " = 10 char
static void printCell2(const std::string& info = "") {
    std::string p = info; p.resize(8, ' ');
    std::cout << "|" << p.substr(0, 8) << " ";
}

// Baris 2 kanan (dengan | penutup)
static void printCell2R(const std::string& info = "") {
    std::string p = info; p.resize(8, ' ');
    std::cout << "|" << p.substr(0, 8) << "|";
}

// Border penuh n sel
static void printBorderFull(int n = NCOLS) {
    for (int i = 0; i < n; i++) std::cout << "+----------";
    std::cout << "+" << std::endl;
}

// Border tengah: "+----------+" + 96 spasi + "+----------+"
static void printBorderMid() {
    std::cout << "+----------+" << std::string(96, ' ') << "+----------+" << std::endl;
}

// Area tengah: selalu MID_W char
static void printMid(const std::string& s) {
    std::ostringstream oss;
    oss << std::left << std::setw(MID_W) << s;
    std::cout << oss.str();
}

// ── Build info baris-2 dari state game ───────────────────────────────────────
// Format: "PX ^"  (pemilik + level bangunan)
// Untuk keperluan testing, ini mengembalikan string dummy
static std::string getTileInfo(int /*tileIdx*/,
                               const std::vector<Player*>& /*players*/) {
    return "";  // Diisi oleh implementasi game logic
}

// ── Build legenda tengah ──────────────────────────────────────────────────────
static std::vector<std::string> buildLegend(int turn, int maxTurn) {
    return {
        "  ==================================",
        "  ||        NIMONSPOLI            ||",
        "  ==================================",
        "              TURN " + std::to_string(turn) + " / " + std::to_string(maxTurn),
        "  --------------------------------",
        "  LEGENDA KEPEMILIKAN & STATUS",
        "  P1-P4 : Properti milik Pemain 1-4",
        "  ^      : Rumah Level 1",
        "  ^^     : Rumah Level 2",
        "  ^^^    : Rumah Level 3",
        "  *      : Hotel (Maksimal)",
        "  (1)-(4): Bidak (IN=Thn,V=Mampir)",
        "  --------------------------------",
        "  KODE WARNA:",
        "  [CK]=Coklat    [MR]=Merah",
        "  [BM]=Biru Muda [KN]=Kuning",
        "  [PK]=Pink      [HJ]=Hijau",
        "  [OR]=Orange    [BT]=Biru Tua",
        "  [DF]=Aksi/Spesial [AB]=Utilitas",
        ""
    };
}

// ── Render papan utama ────────────────────────────────────────────────────────
// Signature sesuaikan dengan header BoardRenderer.hpp di proyek kalian.
// Parameter: tileCodes (40 kode petak), currTurn, maxTurn, players (untuk info baris-2)
void BoardRenderer::renderBoard(
    const std::vector<std::string>& /*tileCodes*/,   // sudah di-hardcode di TILES
    int currTurn,
    int maxTurn,
    const std::vector<Player*>& players)
{
    auto legend = buildLegend(currTurn, maxTurn);
    auto getLeg = [&](int r) -> std::string {
        return (r < (int)legend.size()) ? legend[r] : "";
    };

    // ── BLOK ATAS: petak 21..31 (kiri ke kanan) ──────────────────────────────
    printBorderFull();

    for (int i = 21; i <= 31; i++) printCell1(i);
    std::cout << "|" << std::endl;

    for (int i = 21; i <= 31; i++) printCell2(getTileInfo(i, players));
    std::cout << "|" << std::endl;

    printBorderFull();

    // ── BLOK TENGAH: kiri 20..12 (atas→bawah), kanan 32..40 (atas→bawah) ─────
    int lr = 0;
    for (int i = 0; i < 9; i++) {
        int L = 20 - i;   // 20,19,18,17,16,15,14,13,12
        int R = 32 + i;   // 32,33,34,35,36,37,38,39,40

        // Baris 1: [TAG] kode
        printCell1(L);
        std::cout << "|";
        printMid(getLeg(lr++));
        printCell1R(R);
        std::cout << std::endl;

        // Baris 2: info pemilik/bidak
        printCell2(getTileInfo(L, players));
        std::cout << "|";
        printMid(getLeg(lr++));
        printCell2R(getTileInfo(R, players));
        std::cout << std::endl;

        if (i < 8) printBorderMid();
    }

    // ── BLOK BAWAH: petak 11..1 (tampil kiri ke kanan) ───────────────────────
    printBorderFull();

    for (int i = 11; i >= 1; i--) printCell1(i);
    std::cout << "|" << std::endl;

    for (int i = 11; i >= 1; i--) printCell2(getTileInfo(i, players));
    std::cout << "|" << std::endl;

    printBorderFull();
}