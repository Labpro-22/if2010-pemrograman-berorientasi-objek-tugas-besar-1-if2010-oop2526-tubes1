# GUI Implementation Plan — Nimonspoli
*Last updated: 2026-04-23 | Branch: feat/gui*

---

## 1. Status Quo (Sudah Selesai)

| Komponen | Status | Catatan |
|---|---|---|
| `IGameView` (33 pure-virtual) | ✅ Done | Semua method + DTOs lengkap |
| `IGameInput` (13 pure-virtual) | ✅ Done | Semua prompt type |
| `UiDtos.hpp` | ✅ Done | Semua DTO penuh |
| `GameStateView` (read-only struct) | ✅ Done | Standalone, tidak tunggu Task 5 |
| `CLIView` + `CLIInput` | ✅ Done | Full CLI baseline |
| `HumanController` | ✅ Done | Routing semua prompt ke IGameInput |
| `GUIView::showBoard()` — MVP | ✅ Done | Tile rectangles + color strips + tokens |
| `GUIInput` state machine | ✅ Done | Semua 13 prompt type, render callback |
| `main.cpp` wiring | ✅ Done | Event loop + render callback terhubung |

**Yang belum ada:** Landing page, screen state machine, font rendering, tile textures, HUD panels, log panel, player controller panel.

---

## 2. Asset Inventory

### Font (tersedia)
```
assets/fonts/GemunuLibre/GemunuLibre-Bold.ttf       ← tile names, status, HUD
assets/fonts/GemunuLibre/GemunuLibre-ExtraBold.ttf  ← emphasis
assets/fonts/Hahmlet/hahmlet-bold.ttf               ← title NIMONS POLY, heading besar
```

### Tiles (tersedia)
```
assets/tiles/property/    → BDG BKS BLP BGR DEN DPK GRT IKN JKT MAL
                            MED MGL MKS MND MTR PKB PLB SBY SMG SOL TSK YOG
assets/tiles/railroad/    → GBR GUB STB TGU
assets/tiles/utility/     → PAM PLN
assets/tiles/special/     → GO PPJ PEN BBP
assets/tiles/             → DNU FES KSP PBM PPH  (root = action tiles)
```
> Catatan: `PEN` = Free Parking/Rest, `BBP` = Bebas Parkir?, `PPJ` = Pajak Penghasilan,
> `PPH` = Pajak Penghasilan (cek duplikasi dengan Task 5), `KSP` = Kartu Kesempatan,
> `DNU` = Dana Umum, `FES` = Festival, `PBM` = Pajak Barang Mewah

### Yang Belum Ada
- Background globe/earth texture untuk landing page
- Player token sprites / character icons

---

## 3. Visual Design Reference (dari Figma)

### Palet Warna Global
```
Background luar   : #1a2744  (dark navy)
Globe center      : dark blue dengan earth texture
Panel (neumorphic): #d0d8e4 base, shadow gelap/terang dua sisi
Panel bg dark     : #2a3a5c (game log)
Text utama        : White (#ffffff)
Text sekunder     : #a0b0c0
Accent/hover      : semi-transparent white overlay
```

### Warna Per Grup Properti
```cpp
COKLAT     → #daccc1  (BROWN)
BIRU_MUDA  → #2ec7ff  (LIGHT_BLUE)
MERAH_MUDA → #ee2a89  (PINK)
ORANGE     → rgba(255,119,0,0.74)
MERAH      → #be171a  (RED)
KUNING     → #fff200  (YELLOW)
HIJAU      → #009364  (GREEN)
BIRU_TUA   → #124872  (DARK_BLUE)
```

### Tile Card Structure (100×150px di Figma → scaled di SFML)
```
┌──────────────────┐
│   Foto Kota      │  ← atas 57%: sf::Texture dari assets/tiles/
│  (gradient bg)   │    fallback: solid gradient warna grup
├──────────────────┤
│  ┌────────────┐  │  ← bawah: price badge (white rect)
│  │  M700      │  │    teks warna sesuai grup
│  └────────────┘  │
└──────────────────┘
Border radius ~6px | neumorphic shadow tipis
```

---

## 4. Screen State Machine

```
┌─────────────────────────────────────────────┐
│              AppScreen enum                  │
│                                             │
│  LANDING ──►  NEW_GAME_NUM_PLAYERS          │
│     │              │                        │
│     │              ▼                        │
│     │         NEW_GAME_CUST_PLAYER          │
│     │              │                        │
│     │              ▼                        │
│     │         NEW_GAME_CUST_MAP             │
│     │              │                        │
│     ▼              ▼                        │
│  LOAD_GAME ──► IN_GAME ◄───────────────────┘
│                   │
│                   ▼
│               GAME_OVER
└─────────────────────────────────────────────┘
```

### Enum & State Struct
```cpp
// include/ui/AppScreen.hpp  (baru)
enum class AppScreen {
    LANDING,
    NEW_GAME_NUM_PLAYERS,
    NEW_GAME_CUST_PLAYER,
    NEW_GAME_CUST_MAP,
    LOAD_GAME,
    IN_GAME,
    GAME_OVER,
};

struct SetupState {
    int      numPlayers{2};
    int      numTiles{40};
    vector<string> playerNames;
    vector<bool>   isComputer;
    vector<int>    playerColors;   // index ke palet 4 warna
    int      startingMoney{1000};
    string   loadFilePath;
};
```

---

## 5. Implementasi — Urutan Prioritas

### Step A — Font Loader & Texture Cache  *(~1 jam)*
File: `include/ui/AssetManager.hpp` + `src/views/AssetManager.cpp`

```cpp
class AssetManager {
public:
    static AssetManager& get();
    const sf::Font& font(const string& name);   // "bold", "extrabold", "title"
    const sf::Texture* tileTexture(const string& code); // nullptr = fallback
private:
    map<string, sf::Font>    fonts_;
    map<string, sf::Texture> textures_;
};
```

Load path:
- `assets/fonts/GemunuLibre/GemunuLibre-Bold.ttf` → key `"bold"`
- `assets/fonts/GemunuLibre/GemunuLibre-ExtraBold.ttf` → key `"extrabold"`
- `assets/fonts/Hahmlet/hahmlet-bold.ttf` → key `"title"`
- `assets/tiles/property/<CODE>.png` → key `"<CODE>"`
- `assets/tiles/railroad/<CODE>.png`, `utility/`, `special/`, root `tiles/`
- Fallback: return `nullptr`, GUIView fallback ke gradient warna

---

### Step B — GUIView Refactor: Tile Rendering dengan Texture  *(~2 jam)*
File: `src/views/GUIView.cpp`

Ubah `showBoard()` untuk render setiap tile sebagai:
1. Rounded rect background (warna grup gradient)
2. `sf::Sprite` dari `AssetManager::tileTexture(code)` — clip ke area atas 57%
3. Price badge rect putih di bawah (hanya untuk property/railroad/utility)
4. Text kode/nama tile dengan font GemunuLibre-Bold, scale ke ukuran tile

Corner tiles (GO, JAIL, GOTOJAIL, REST) → 150×150, full texture

Tile yang dirotasi (sisi kiri & kanan board) → `sf::Transform` rotate 90° / -90°

---

### Step C — Screen State Machine + GUIView::showMainMenu()  *(~3 jam)*

Tambah di `GUIView`:
```cpp
// Render sesuai AppScreen saat ini
void renderScreen(AppScreen screen, const SetupState& setup);
```

Internal draw methods:
```cpp
void drawLandingPage();
void drawNumPlayers(const SetupState&);
void drawCustPlayer(const SetupState&, int selectedPlayer);
void drawCustMap(const SetupState&);
void drawLoadGame(const string& filePath);
```

**Landing Page** (dari Figma `151:1520`):
- Background: `#1a2744` solid fill
- Globe: `sf::CircleShape` besar radius ~40% layar, fill dark blue,
  radial glow (white semi-transparent circle gradient) sebagai "atmosphere"
- Title: "NIMONS" + newline + "POLY" font Hahmlet-Bold, putih, center
- Subtitle: "presented by BurntCheesecake", kecil
- ♠ kiri, ♣ kanan (unicode rendered as sf::Text)
- Menu items: "New Game", "Load Game", "Credits", "Exit"
  → `sf::Text` centered, highlight on hover (mouse over = brighter)
  → klik = state transition

**Num Players** (dari Figma `136:593`):
- Background sama (globe)
- Label box: semi-transparent dark rect + "Choose number of player"
- 3 opsi: "2 Players" / "3 Players" / "4 Players"
  → selected = highlight putih, unselected = redup
- Tombol "Next"

**Cust Player** (dari Figma `136:1303`):
- Header: "< Customize Player" (klik < = back)
- Left sidebar: tab Player #1..#N, klik untuk switch
- Right form per player:
  - Name: `sf::RectangleShape` input box (GUIInput-style text buffer)
  - Colors: 4 colored squares (cyan/pink/yellow/green) → klik pilih
  - Human/Computer: dua checkbox
  - Starting Money: input box
- Tombol "Next"

**Cust Map** (dari Figma `136:1344`):
- Header: "< Customize Map"
- Grid tombol tile count: 20,24,28,32,36,40,44,48,52,56,60
  → selected = bright, default = gray
- Tombol "Start" → transisi ke IN_GAME

**Load Game** (dari Figma `144:1466`):
- Label: "Load config file"
- Input box text (ketik path file)
- Tombol "Start"

---

### Step D — In-Game HUD Layout  *(~4 jam)*

Layout 2560×2000 di Figma → scale ke window 1440×900 (default SFML):

```
┌──────────────┬────────────────────────┬──────────────┐
│ Left Panel   │     Board (center)     │ Right Panel  │
│  200px wide  │    ~800×800px board    │  250px wide  │
│              │                        │              │
│ Game Summary │   (40 tiles + center   │  Game Log    │
│ Round/Time   │    globe logo)         │  (scrollable)│
│              │                        │              │
│ Player Tokens│                        │  Action Btns │
│              │                        │  (4 slots)   │
│ Players List │                        │              │
│              │                        │  Lempar Dadu │
├──────────────┴────────────────────────┤  (large btn) │
│        Your Property (full width)     │              │
└───────────────────────────────────────┴──────────────┘
```

**Left Panel** (neumorphic gray `#d0d8e4`):
- "Game Summary" card:
  - Round: `currentTurn` / `maxTurn`
  - Player token mini-icons (4 buah, colored circles + nama inisial)
- "Players" list: per player → nama, uang, status (BANKRUPT = redup), highlight = giliran aktif

**Right Panel**:
- "Game Log" dark box (`#2a3a5c`):
  - Scrollable list log entries — format: `[PlayerName] action detail`
  - Last N entries, scroll otomatis ke bawah saat update
- 4 Action Buttons (abu-abu neumorphic, 2×2 grid):
  - Slot ini dinamis sesuai konteks (giliran aktif menampilkan aksi yang tersedia)
  - Contoh: "Beli", "Gadai", "Bangun", "Gunakan Kartu"
- "Lempar Dadu" button besar (biru gelap, teks Hahmlet-Bold)
  → Disabled setelah dadu dilempar, enabled saat giliran baru

**Your Property** panel (bawah, full-width):
- Horizontal scroll dari property card mini (thumbnail tile + nama + level bangunan)
- Hanya tampilkan milik current player

---

### Step E — GUIInput Overlay Rendering  *(~2 jam)*

Saat `GUIInput` blocking (prompt aktif), render overlay di atas board:
- Semi-transparent dark rect (seluruh layar, alpha ~160)
- Prompt box di tengah: label + input buffer + instruksi
- Type-specific rendering:
  - `YES_NO`: dua tombol [Y] / [N]
  - `MENU_CHOICE`: daftar opsi bernomor (klik atau tekan angka)
  - `TEXT`/`COMMAND`: text field dengan cursor blink
  - `DICE_MANUAL`: dua slot angka (D1, D2)
  - `AUCTION`: label PASS / BID + input amount

Expose `GUIInput::currentPrompt()` ke `GUIView` melalui pointer:
```cpp
// GUIView
void setInput(const GUIInput* input);  // weak reference, nullable
// Di showBoard() atau render loop, cek currentPrompt().type != NONE
```

---

### Step F — main.cpp Full Wiring  *(~1 jam)*

Ubah `main.cpp` agar:
1. Buat `GUIView`, `GUIInput`, `AssetManager`
2. Mulai di `AppScreen::LANDING`
3. Loop:
   - Poll events → GUIInput::handleEvent
   - Render sesuai current screen
   - Klik menu → state transition
   - Saat IN_GAME → jalankan `GameEngine` (dari Task 5) di thread/step terpisah

```cpp
// Pseudocode
AppScreen screen = AppScreen::LANDING;
SetupState setup;
while (window.isOpen()) {
    pollEvents();
    switch (screen) {
        case LANDING:       view.drawLandingPage(); break;
        case NUM_PLAYERS:   view.drawNumPlayers(setup); break;
        // ...
        case IN_GAME:       view.showBoard(gameState); break;
    }
    window.display();
}
```

---

### Step G — ComputerController  *(Bonus, last)*

`include/controllers/ComputerController.hpp`

```cpp
class ComputerController : public PlayerController {
public:
    AuctionDecision decideAuction(int currentBid, int myMoney) override;
    int decideBuild(const BuildMenuState& state) override;
    // dll — semua pure-virtual dari PlayerController
private:
    // Strategi sederhana: greedy (beli selalu jika mampu, bid 10% di atas current)
};
```

---

## 6. File Baru yang Akan Dibuat

| File | Tujuan |
|---|---|
| `include/ui/AppScreen.hpp` | Enum AppScreen + SetupState |
| `include/ui/AssetManager.hpp` | Font & texture cache |
| `src/views/AssetManager.cpp` | Implementasi loader |
| `include/controllers/ComputerController.hpp` | COM player |
| `src/controllers/ComputerController.cpp` | Implementasi COM |

---

## 7. File yang Dimodifikasi

| File | Perubahan |
|---|---|
| `src/views/GUIView.cpp` | Tambah semua draw methods, texture rendering |
| `include/ui/GUIView.hpp` | Tambah signature draw methods |
| `src/main.cpp` | Full screen state machine |
| `include/ui/GUIInput.hpp` | Tambah `setInput` pointer untuk overlay |

---

## 8. Urutan Pengerjaan Berdasarkan Prioritas Deadline

```
[HARI INI — 2026-04-23]
  A. AssetManager (font + texture loader)
  B. GUIView tile rendering dengan texture

[ESOK — 2026-04-24, deadline]
  C. Landing Page + screen state machine
  D. In-Game HUD (Game Summary, Players, Log, Lempar Dadu)
  E. Prompt overlay rendering
  F. main.cpp full wiring

[BONUS — jika sempat]
  G. ComputerController
  H. Cust Player / Cust Map screens (detail)
  I. Dynamic board (tile count != 40)
```

---

## 9. Hal Penting / Constraints

- **Working directory** saat run = project root (`${CMAKE_SOURCE_DIR}`)
  → semua path asset: `"assets/fonts/..."`, `"assets/tiles/..."`
- **Tile codes** harus match dengan yang dipakai Task 5 (property.txt): GRT, TSK, GBR, BGR, DPK, BKS, MGL, SOL, YOG, STB, MAL, SMG, SBY, MKS, BLP, MND, TGU, PLB, PKB, MED, BDG, DEN, MTR, GUB, JKT, IKN, PLN, PAM, + spesial
- **Railroad code**: Figma pakai "GBR/STB/TGU/GUB", context.md pakai GBR/STB/TUG/GUB → konfirmasi ke Task 5 tim soal TGU vs TUG
- **GUIView tidak boleh punya game logic** — hanya baca `GameStateView` dan DTOs
- **Thread safety**: GameEngine (Task 5) akan jalan di thread berbeda dari render loop → koordinasi via `std::atomic<bool>` atau mutex pada `GameStateView` snapshot
- **Fallback**: Jika texture tidak ada → render gradient warna grup saja. Game harus tetap jalan tanpa assets.
