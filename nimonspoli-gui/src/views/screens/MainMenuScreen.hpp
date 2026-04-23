#pragma once
#include "../IScreen.hpp"
#include "../Window.hpp"
#include "../../../lib/raylib/include/raylib.h"
#include <string>
#include <vector>
#include <array>

// ─── Data yang dikumpulkan dari main menu ────────────────────────────────────
struct GameSetup {
    int playerCount;                    // 2-4
    std::array<std::string, 4> names;   // username tiap slot
    std::string saveFile;               // kosong = new game, isi = load game
    bool isLoadGame;
};

// ─── MainMenuScreen ──────────────────────────────────────────────────────────
class MainMenuScreen : public IScreen {
public:
    MainMenuScreen();
    ~MainMenuScreen() override;

    void onEnter() override;
    void onExit()  override;
    void update(float dt) override;
    void render(Window& window) override;

    bool isReadyToStart() const { return readyToStart; }
    const GameSetup& getSetup() const { return setup; }
    void resetReady(){ readyToStart = false; }

private:
    // ── Konstanta layar ───────────────────────────────────────────────────
    static constexpr int SCREEN_W = 1920;
    static constexpr int SCREEN_H = 1080;

    Color playerColors[4] = {
        {220,  50,  50, 255},
        {240, 200,  50, 255},
        { 50, 180,  50, 255},
        { 50, 200, 220, 255},
    };

    // ── State UI ──────────────────────────────────────────────────────────
    int   selectedPlayerCount;
    int   focusedInput;
    float glowTimer;

    std::string nameBuffers[4];

    // ── Save file state ───────────────────────────────────────────────────
    bool                     saveFileExists;
    std::vector<std::string> saveFiles;      // daftar semua *.txt di data/
    int                      selectedSaveIdx; // index yang dipilih di load panel

    // ── Load panel state ──────────────────────────────────────────────────
    bool  showLoadPanel;    // true = tampilkan overlay pilih save
    double lastClickTime;   // untuk deteksi double-click

    // ── Output state ──────────────────────────────────────────────────────
    bool      readyToStart;
    GameSetup setup;

    // ── Error state ───────────────────────────────────────────────────────
    std::string errorMsg;
    float       errorTimer;

    // ── Private methods ───────────────────────────────────────────────────
    void handleInput();
    void drawTitle();
    void drawLeftPanel();
    void drawRightPanel();
    void drawLoadPanel();   // overlay modal: pilih dari daftar save
    void drawError();

    bool validateInputs();
    void scanSaveFiles();           // scan data/*.txt ke saveFiles[]
    void triggerLoad(const std::string& path);
    void applySetup();
};