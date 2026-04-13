#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#define RST  "\033[0m"
#define C_CK "\033[38;5;130m"
#define C_BM "\033[38;5;117m"
#define C_PK "\033[38;5;213m"
#define C_OR "\033[38;5;214m"
#define C_MR "\033[31m"
#define C_KN "\033[33m"
#define C_HJ "\033[32m"
#define C_BT "\033[38;5;27m"
#define C_AB "\033[38;5;245m"
#define C_DF "\033[38;5;240m"

// ── Ukuran papan ─────────────────────────────────────────────────
// Semua baris = 122 char
// Sel penuh    : "|" + 10 char konten = 11 char
// Baris penuh  : 11 sel × 11 + "|" akhir = 122 ✓
// Border penuh : 11 × "+----------" + "+" = 122 ✓
// Baris tengah : kiri(11) + "|"(1) + MID(99) + kanan_9char + "|"(1+1) = 122 ✓
//                  ["+----------+"(12) + sp(98) + "+----------+"(12) = 122]
// ─────────────────────────────────────────────────────────────────
static const int MID_W  = 99;
static const int MID_SP = 98; // spasi di border tengah

struct Tile { std::string code, tag; const char* color; };
static Tile T[41] = {
    {},
    {"GO","[DF]",C_DF},{"GRT","[CK]",C_CK},{"DNU","[DF]",C_DF},
    {"TSK","[CK]",C_CK},{"PPH","[DF]",C_DF},{"GBR","[DF]",C_DF},
    {"BGR","[BM]",C_BM},{"FES","[DF]",C_DF},{"DPK","[BM]",C_BM},
    {"BKS","[BM]",C_BM},{"PEN","[DF]",C_DF},{"MGL","[PK]",C_PK},
    {"PLN","[AB]",C_AB},{"SOL","[PK]",C_PK},{"YOG","[PK]",C_PK},
    {"STB","[DF]",C_DF},{"MAL","[OR]",C_OR},{"DNU","[DF]",C_DF},
    {"SMG","[OR]",C_OR},{"SBY","[OR]",C_OR},{"BBP","[DF]",C_DF},
    {"MKS","[MR]",C_MR},{"KSP","[DF]",C_DF},{"BLP","[MR]",C_MR},
    {"MND","[MR]",C_MR},{"TUG","[DF]",C_DF},{"PLB","[KN]",C_KN},
    {"PKB","[KN]",C_KN},{"PAM","[AB]",C_AB},{"MED","[KN]",C_KN},
    {"PPJ","[DF]",C_DF},{"BDG","[HJ]",C_HJ},{"DEN","[HJ]",C_HJ},
    {"FES","[DF]",C_DF},{"MTR","[HJ]",C_HJ},{"GUB","[DF]",C_DF},
    {"KSP","[DF]",C_DF},{"JKT","[BT]",C_BT},{"PBM","[DF]",C_DF},
    {"IKN","[BT]",C_BT},
};

// ── State game dinamis ────────────────────────────────────────────
struct TileState {
    std::string owner;        // "" / "P1".."P4"
    int buildings = 0;        // 0-4=rumah, 5=hotel
    std::vector<int> players; // bidak pemain di sini
    bool mortgaged = false;
};
std::map<int, TileState> GS; // GS = GameState

// Build info baris-2 petak (max 9 char konten)
std::string info(int idx) {
    auto it = GS.find(idx);
    if (it == GS.end()) return "";
    auto& s = it->second;
    std::string r = "";
    if (!s.owner.empty()) {
        r += s.owner + " ";
        if      (s.mortgaged)   r += "[M]";
        else if (s.buildings==5) r += "*";
        else if (s.buildings>0)  r += std::string(s.buildings,'^');
    }
    for (int p : s.players) {
        if (!r.empty()) r += " ";
        r += "("+std::to_string(p)+")";
    }
    if (r.size() > 9) r = r.substr(0,9);
    return r;
}

// ── Helpers cetak ─────────────────────────────────────────────────

// Baris-1 sel biasa: "|[TAG] COD  " = 11 char (| + 10 konten)
// [TAG]=4, " "=1, code=3, "  "=2 → 4+1+3+2=10 konten ✓
void p1(int i) {
    std::string c = T[i].code; c.resize(3,' ');
    std::cout << "|" << T[i].color << T[i].tag << " " << c << RST << "  ";
}

// Baris-2 sel biasa: "|info       " = 11 char (| + 9 konten + " ")
void p2(int i) {
    std::string v = info(i); v.resize(9,' ');
    std::cout << "|" << v << " ";
}

// Baris-1 kanan (dalam blok tengah): "[TAG] COD  |" = 10 char (tanpa | di depan)
// Dipanggil setelah "|" dari MID, jadi total: MID_"|"(1) + kanan(9) + "|"(1) = 11
void p1R(int i) {
    std::string c = T[i].code; c.resize(3,' ');
    std::cout << T[i].color << T[i].tag << " " << c << RST << " |";
    // 4+1+3+1 = 9 + "|" = 10 char, benar sebagai penutup kanan
}

// Baris-2 kanan: "info      |" = 10 char
void p2R(int i) {
    std::string v = info(i); v.resize(9,' ');
    std::cout << v << "|";
}

void borderFull() {
    for (int i=0;i<11;i++) std::cout<<"+----------";
    std::cout<<"+"<<std::endl;
}

void borderMid() {
    std::cout<<"+----------+"<<std::string(MID_SP,' ')<<"+----------+"<<std::endl;
}

void mid(const std::string& s) {
    std::ostringstream o; o<<std::left<<std::setw(MID_W)<<s;
    std::cout<<o.str();
}

void renderBoard(int turn, int maxTurn=50) {
    std::vector<std::string> leg = {
        "  ================================",
        "  ||        NIMONSPOLI          ||",
        "  ================================",
        "            TURN "+std::to_string(turn)+" / "+std::to_string(maxTurn),
        "  ------------------------------",
        "  LEGENDA KEPEMILIKAN & STATUS",
        "  P1-P4 : Properti milik Pemain 1-4",
        "  ^      : Rumah Level 1",
        "  ^^     : Rumah Level 2",
        "  ^^^    : Rumah Level 3",
        "  *      : Hotel (Maksimal)",
        "  (n)    : Bidak pemain ke-n",
        "  [M]    : Properti digadaikan",
        "  IN=Tahanan  V=Mampir (di Penjara)",
        "  ------------------------------",
        "  KODE WARNA:",
        "  [CK]=Coklat    [MR]=Merah",
        "  [BM]=Biru Muda [KN]=Kuning",
        "  [PK]=Pink      [HJ]=Hijau",
        "  [OR]=Orange    [BT]=Biru Tua",
        "  [DF]=Aksi/Spesial  [AB]=Utilitas",
        ""
    };
    auto L=[&](int r)->std::string{ return r<(int)leg.size()?leg[r]:""; };

    // ── ATAS: 21..31 ──────────────────────────────────────────────
    borderFull();
    for(int i=21;i<=31;i++) p1(i); std::cout<<"|\n";
    for(int i=21;i<=31;i++) p2(i); std::cout<<"|\n";
    borderFull();

    // ── TENGAH: kiri 20..12, kanan 32..40 ────────────────────────
    // Baris tengah: kiri(11) + "|"(1) + mid(99) + kanan_9 + "|"(1+1) = 122
    int lr=0;
    for(int i=0;i<9;i++){
        int L_=20-i, R_=32+i;
        p1(L_); std::cout<<"|"; mid(L(lr++)); std::cout<<"|"; p1R(R_); std::cout<<"\n";
        p2(L_); std::cout<<"|"; mid(L(lr++)); std::cout<<"|"; p2R(R_); std::cout<<"\n";
        if(i<8) borderMid();
    }

    // ── BAWAH: 11..1 ─────────────────────────────────────────────
    borderFull();
    for(int i=11;i>=1;i--) p1(i); std::cout<<"|\n";
    for(int i=11;i>=1;i--) p2(i); std::cout<<"|\n";
    borderFull();
}

// ── Demo state ────────────────────────────────────────────────────
void initDemo() {
    GS[38] = {"P1", 1, {4}};   // JKT: P1 1rumah, bidak P4
    GS[32] = {"P2", 2, {}};    // BDG: P2 2rumah
    GS[11] = {"",   0, {3}};   // PEN: bidak P3
    GS[ 1] = {"",   0, {1}};   // GO : bidak P1
    GS[22] = {"P2", 5, {}};    // MKS: P2 hotel
    GS[33] = {"P3", 0, {}, true}; // DEN: P3 gadai
}

int main(){
    initDemo();
    std::cout<<"\033[2J\033[H";
    std::cout<<"--- TESTING DISPLAY NIMONSPOLI MILSTONE 1 ---\n";
    renderBoard(15,50);
    return 0;
}