#pragma once
#include "../../../lib/raylib/include/raylib.h"
#include "../IScreen.hpp"
#include "../../core/utils/TransactionLogger.hpp"
#include "../GUIManager.hpp"
#include "../../core/Property/StreetProperty.hpp"
#include "../../core/Card/SkillCard.hpp"
#include "../../core/Card/MoveCard.hpp"
#include "../../core/Card/DiscountCard.hpp"
#include "../../core/Card/ShieldCard.hpp"
#include "../../core/Card/TeleportCard.hpp"
#include "../../core/Card/LassoCard.hpp"
#include "../../core/Card/DemolitionCard.hpp"

#include <string>
#include <vector>
#include <map>

// ─── Mock data structs (replace with real backend later) ───────────────────
struct MockProperty {
    std::string code;
    std::string name;
    std::string colorGroup;
    std::string type;         // "STREET","RAILROAD","UTILITY","ACTION"
    int         owner;        // -1=bank, 0-3=player index
    int         buildings;    // 0-4 houses, 5=hotel
    bool        mortgaged;
    int         festivalMult; // 1,2,4,8
    int         festivalDur;
    int rentL0,rentL1,rentL2,rentL3,rentL4,rentL5;
    int price, mortgageVal, houseUpg, hotelUpg;
};

struct MockPlayer {
    std::string username;
    int         money;
    int         position;    // tile render index 0-39
    std::string status;      // "ACTIVE","JAILED","BANKRUPT"
    int         cardCount;
    bool        isCurrentTurn;
};

struct MockGameState {
    int currentTurn;
    int maxTurn;
    int activePlayerIdx;
    std::vector<MockPlayer>   players;
    std::vector<MockProperty> properties;
    TransactionLogger logger;
    bool kspGlow;
    bool dnuGlow;
};

struct TileDef {
    std::string code;
    std::string side;   // "BOTTOM","LEFT","TOP","RIGHT"
    bool        corner;
};

// ─── GameScreen : public IScreen ──────────────────────────────────────────
class GameScreen : public IScreen {
public:
    GameScreen();
    ~GameScreen() override;

    
    void onEnter() override;
    void onExit()  override;
    void update(float dt) override;
    void render(Window& window) override;
    void setPlayerCount(int n) { activePlayerCount = n; }
    void setGUIManager(GUIManager* gm) { guiManager = gm; }
    int  activePlayerCount = 4;
    bool gameOver = false;
    bool isGameOver() const { return gameOver; }

    // Sinkronisasi dengan GameMaster (mode real)
    void syncFromGameMaster();
    bool isRealMode() const;
    void syncDiceResult();

    // ── Player colors ────────────────────────────────────────────────────
    Color playerColors[4] = {
        {220,  50,  50, 255},  // P1 red
        {240, 200,  50, 255},  // P2 yellow
        { 50, 180,  50, 255},  // P3 green
        { 50, 200, 220, 255},  // P4 cyan
    };

private:

    // ── Layout ──────────────────────────────────────────────────────────
    static constexpr int   SCREEN_W    = 1920;
    static constexpr int   SCREEN_H    = 1080;
    static constexpr int   LEFT_PANEL  = 270;
    static constexpr int   RIGHT_PANEL = 270;

    static constexpr float ORIG_TILE_W  = 200.f;
    static constexpr float ORIG_TILE_H  = 290.f;
    static constexpr float ORIG_CORNER  = 290.f;
    static constexpr float ORIG_STRIP_W = 200.f;
    static constexpr float ORIG_STRIP_H =  35.f;

    static constexpr float SCALE      = 1080.f / 2380.f;
    static constexpr float TILE_W     = ORIG_TILE_W  * SCALE;
    static constexpr float TILE_H     = ORIG_TILE_H  * SCALE;
    static constexpr float CORNER_SZ  = ORIG_CORNER  * SCALE;
    static constexpr float STRIP_W    = ORIG_STRIP_W * SCALE;
    static constexpr float STRIP_H    = ORIG_STRIP_H * SCALE;

    float boardX, boardY;

    // ── Tiles ────────────────────────────────────────────────────────────
    std::vector<TileDef> tiles;

    // ── Textures ─────────────────────────────────────────────────────────
    std::map<std::string, Texture2D> tileTextures;
    Texture2D deckKSP{}, deckDNU{};

    // ── Zoom (center only) ───────────────────────────────────────────────
    float   zoomLevel;
    Vector2 zoomOffset;
    bool    isDragging;
    Vector2 dragStart;

    // ── Popup ────────────────────────────────────────────────────────────
    int  selectedTile;
    bool showPopup;

    // ── Glow ─────────────────────────────────────────────────────────────
    bool  kspGlowing, dnuGlowing;
    float glowTimer;

    // ── State ────────────────────────────────────────────────────────────
    MockGameState gameState;

    // ── GUIManager ───────────────────────────────────────────────────────
    GUIManager* guiManager = nullptr;

    // ── Player Visual (animasi pion) ─────────────────────────────────────
    struct PlayerVisual {
        float currentTileIdx;
        float targetTileIdx;
    };
    std::vector<PlayerVisual> playerVisuals;

    // ── State dadu ───────────────────────────────────────────────────────
    struct DiceState {
        int val1 = 0, val2 = 0;
        bool hasRolled   = false;
        bool isDouble    = false;
        bool tripleDouble = false;
        float animTimer  = 0.f;
        bool animating   = false;
        static constexpr float ANIM_DURATION = 0.6f;
    } diceState;

    // ── Save Popup ────────────────────────────────────────────────────────
    struct SavePopup {
        bool    visible          = false;
        bool    confirmOverwrite = false;
        bool    resultVisible    = false;
        bool    resultOk         = false;
        float   resultTimer      = 0.f;
        std::string fileNameInput = "save";
        std::string resultMsg;
    } savePopup;

    // ── Log Popup ─────────────────────────────────────────────────────────
    bool showLogPopup;
    int  logShowN;
    float logScrollY;
    std::string logNInput;
    bool logNFocused;

    // ── Buy Dialog ────────────────────────────────────────────────────────
    struct BuyDialogState {
        bool visible   = false;
        int  tileIdx   = -1;
        bool canAfford = true;
    } buyDialog;

    // ── Tax Dialog ────────────────────────────────────────────────────────
    struct TaxDialogState {
        bool visible       = false;
        int  flatAmount    = 0;
        int  pctAmount     = 0;
        int  wealth        = 0;
        bool canAffordFlat = true;
        bool canAffordPct  = true;
        int  taxAmtPct     = 0;
    } taxDialog;

    // ── Festival Dialog ───────────────────────────────────────────────────
    struct FestivalDialogState {
        bool visible   = false;
        float scrollY  = 0.f;
        std::vector<StreetProperty*> streets;
        int hoveredIdx = -1;
    } festivalDialog;

    // ── Card Dialog ───────────────────────────────────────────────────────
    struct CardDialogState {
        bool visible          = false;
        std::string deckLabel;
        std::string description;
    } cardDialog;

    // Jail Dialog
    

    void triggerCardDialog();   // dipanggil saat phase == SHOW_CARD
    void drawCardDialog();      // render popup panel + tombol OK

    // ── Dialog Gadai ──────────────────────────────────────────────────────
    struct GadaiDialogState {
        bool visible = false;
        float scrollY = 0.f;
        // list PropertyTile* yang bisa digadaikan
        struct Entry { Property* prop; int tileIdx; };
        std::vector<Entry> entries;
        int hoveredIdx = -1;
    } gadaiDialog;
    void triggerGadaiDialog();
    void drawGadaiDialog();

    // ── Dialog Tebus ──────────────────────────────────────────────────────
    struct TebusDialogState {
        bool visible = false;
        float scrollY = 0.f;
        struct Entry { Property* prop; int tileIdx; };
        std::vector<Entry> entries;
        int hoveredIdx = -1;
    } tebusDialog;
    void triggerTebusDialog();
    void drawTebusDialog();

    // ── Dialog Bangun ─────────────────────────────────────────────────────
    struct BangunDialogState {
        bool visible = false;
        float scrollY = 0.f;
        // Group terpilih: -1 = belum pilih group
        int selectedGroupIdx = -1;
        // Map: colorGroup → list StreetProperty*
        std::vector<std::pair<std::string, std::vector<StreetProperty*>>> groups;
        int hoveredIdx = -1;
    } bangunDialog;
    void triggerBangunDialog();
    void drawBangunDialog();

    // ── Dialog Jual Bangunan ──────────────────────────────────────────────
    struct JualBangunanDialogState {
        bool visible = false;
        float scrollY = 0.f;
        struct Entry { StreetProperty* prop; int tileIdx; };
        std::vector<Entry> entries;
        int hoveredIdx = -1;
    } jualBangunanDialog;
    void triggerJualBangunanDialog();
    void drawJualBangunanDialog();

    // ── Dialog Skill Card (USE CARD) ──────────────────────────────────────
    struct SkillCardDialogState {
        bool visible = false;
        int hoveredIdx = -1;
        // TeleportCard needs tile selection
        bool awaitingTeleportTile = false;
        // LassoCard needs target player selection
        bool awaitingLassoTarget = false;
        int selectedCardIdx = -1;
    } skillCardDialog;
    void triggerSkillCardDialog();
    void drawSkillCardDialog();


    // ── Private methods ──────────────────────────────────────────────────
    Color   getGroupColor(const std::string& group);
    void    loadTextures();
    void    initMockState();
    void    handleInput();

    // ── GameScreenBoard.cpp ───────────────────────────────────────────────
    void    drawBoard();
    void    drawTile(int idx, float cx, float cy, float rotation);
    void    drawBuildingStrip(float cx, float cy, float rotation,
                              int buildings, Color ownerColor);
    void    drawPlayers(int tileIdx, float cx, float cy);
    void    drawCenterArea();
    void    drawPopup();

    Vector2   getTileCenter(int idx);
    Rectangle getTileRect(int idx);
    int       tileAtPoint(Vector2 pt);

    // ── GameScreenPanels.cpp ──────────────────────────────────────────────
    void    drawLeftPanel();
    void    drawRightPanel();

    // ── GameScreenDice.cpp ────────────────────────────────────────────────
    void    drawDiceArea();
    void    handleLemparDadu();

    // ── GameScreenDialogBuy.cpp ───────────────────────────────────────────
    void    triggerBuyDialog(int tileIdx);
    void    drawBuyDialog();

    // ── GameScreenDialogTax.cpp ───────────────────────────────────────────
    void    triggerTaxDialog();
    void    drawTaxDialog();

    // ── GameScreenDialogFestival.cpp ──────────────────────────────────────
    void    triggerFestivalDialog();
    void    drawFestivalDialog();

    // ── GameScreenDialogCard.cpp ──────────────────────────────────────────
    // void    triggerCardDialog();
    // void    drawCardDialog();

    // ── GameScreenDialogJail.cpp ──────────────────────────────────────────
    void    triggerJailDialog();
    void    drawJailDialog();

    // ── GameScreenDialogProperti.cpp ──────────────────────────────────────
    void    drawPropertiPopup();

    // ── GameScreenSave.cpp ────────────────────────────────────────────────
    void    handleSimpan();
    void    drawSavePopup();
    void    doSave(const std::string& filepath);

    // ── GameScreenLog.cpp ─────────────────────────────────────────────────
    void    initMockLogs();
    void    drawLogPopup();
    std::string getActionIcon(const std::string& action);
    Color       getActionColor(const std::string& action);
};