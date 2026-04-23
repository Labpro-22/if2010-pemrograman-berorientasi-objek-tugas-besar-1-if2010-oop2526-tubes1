#include "GameScreen.hpp"
#include "../../../lib/raylib/include/raylib.h"
#include "../../core/GameMaster/GameMaster.hpp"
#include "../../core/Player/Player.hpp"
#include <sstream>
#include <string>

// ─── Trigger ─────────────────────────────────────────────────────────────────
void GameScreen::triggerCetakPropertiDialog(int playerIdx)
{
    cetakPropertiDialog.scrollY   = 0.f;
    cetakPropertiDialog.playerIdx = playerIdx;
    cetakPropertiDialog.visible   = true;

    if (isRealMode()) {
        const auto& players = guiManager->getGameMaster()->getState().getPlayers();
        if (playerIdx < 0 || playerIdx >= (int)players.size()) {
            cetakPropertiDialog.content    = "(Pemain tidak ditemukan)";
            cetakPropertiDialog.playerName = "?";
            return;
        }
        Player* p = players[playerIdx];
        cetakPropertiDialog.playerName = p->getUsername();
        cetakPropertiDialog.content    = p->cetakProperti();  // method dari Player.cpp
    } else {
        // Mode mock — tampilkan properti milik pemain ini dari MockGameState
        if (playerIdx < 0 || playerIdx >= (int)gameState.players.size()) return;
        cetakPropertiDialog.playerName = gameState.players[playerIdx].username;

        std::ostringstream oss;
        oss << "=== Properti Milik: " << gameState.players[playerIdx].username << " ===\n\n";
        bool found = false;
        for (int i = 0; i < (int)gameState.properties.size(); i++) {
            if (gameState.properties[i].owner == playerIdx) {
                found = true;
                auto& mp = gameState.properties[i];
                oss << "[" << mp.code << "] " << mp.name << "\n"
                    << "  Harga  : M" << mp.price      << "\n"
                    << "  Gadai  : M" << mp.mortgageVal << "\n"
                    << "  Bangunan: " << (mp.buildings == 5 ? "Hotel"
                                        : mp.buildings > 0 ? std::to_string(mp.buildings)+" rumah"
                                                           : "-") << "\n\n";
            }
        }
        if (!found) oss << "Belum memiliki properti.\n";
        cetakPropertiDialog.content = oss.str();
    }
}

// ─── Render ──────────────────────────────────────────────────────────────────
void GameScreen::drawCetakPropertiDialog()
{
    if (!cetakPropertiDialog.visible) return;

    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, {0,0,0,160});

    constexpr float PW = 580.f, PH = 560.f;
    float px = SCREEN_W/2.f - PW/2.f;
    float py = SCREEN_H/2.f - PH/2.f;

    DrawRectangle((int)px,(int)py,(int)PW,(int)PH,{18,20,30,255});
    DrawRectangleLinesEx({px,py,PW,PH},2.f,{80,130,200,255});

    // Header — warna player
    int pidx = cetakPropertiDialog.playerIdx;
    Color hdrCol = (pidx >= 0 && pidx < 4)
                 ? Color{(unsigned char)(playerColors[pidx].r/2),
                         (unsigned char)(playerColors[pidx].g/2),
                         (unsigned char)(playerColors[pidx].b/2),255}
                 : Color{30,50,80,255};
    DrawRectangle((int)px,(int)py,(int)PW,48,hdrCol);

    // Garis warna pemain di kiri header
    if (pidx >= 0 && pidx < 4)
        DrawRectangle((int)px,(int)py,6,48,playerColors[pidx]);

    std::string title = "PROPERTI — " + cetakPropertiDialog.playerName;
    int tw = MeasureText(title.c_str(),14);
    DrawText(title.c_str(),(int)(px+PW/2-tw/2),(int)(py+16),14,WHITE);

    // Tombol X
    Rectangle xBtn = {px+PW-36,py+10,28,28};
    bool xHov = CheckCollisionPointRec(GetMousePosition(),xBtn);
    DrawRectangleRec(xBtn,xHov?Color{180,60,60,255}:Color{110,40,40,255});
    DrawText("X",(int)(xBtn.x+9),(int)(xBtn.y+8),12,WHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && xHov) {
        cetakPropertiDialog.visible = false; return;
    }

    // ── Scrollable content ────────────────────────────────────────────────
    float listX = px+16, listY = py+56, listW = PW-32, listH = PH-72;

    std::vector<std::string> lines;
    std::istringstream iss(cetakPropertiDialog.content);
    std::string line;
    while (std::getline(iss, line)) lines.push_back(line);

    constexpr float ROW_H = 17.f;
    float totalH = lines.size() * ROW_H;

    Rectangle listArea = {listX,listY,listW,listH};
    if (CheckCollisionPointRec(GetMousePosition(),listArea)) {
        cetakPropertiDialog.scrollY -= GetMouseWheelMove() * 36.f;
        cetakPropertiDialog.scrollY  = std::max(0.f, std::min(cetakPropertiDialog.scrollY,
                                       std::max(0.f, totalH - listH)));
    }

    BeginScissorMode((int)listX,(int)listY,(int)listW,(int)listH);
    for (int i = 0; i < (int)lines.size(); i++) {
        float ry = listY + i*ROW_H - cetakPropertiDialog.scrollY;
        if (ry+ROW_H < listY || ry > listY+listH) continue;

        Color c;
        if (lines[i].find("===") != std::string::npos)
            c = (pidx>=0&&pidx<4) ? playerColors[pidx] : Color{200,200,100,255};
        else if (lines[i].find('[') != std::string::npos)
            c = {220,220,255,255};
        else if (lines[i].find("Hotel") != std::string::npos)
            c = {255,160,80,255};
        else if (lines[i].find("rumah") != std::string::npos)
            c = {150,220,150,255};
        else
            c = {180,180,200,255};

        DrawText(lines[i].c_str(),(int)(listX+4),(int)(ry+2),11,c);
    }
    EndScissorMode();

    // Scrollbar
    if (totalH > listH) {
        float sbH = listH*(listH/totalH);
        float sbY = listY+(cetakPropertiDialog.scrollY/totalH)*listH;
        DrawRectangle((int)(listX+listW-5),(int)listY,4,(int)listH,{40,42,58,255});
        DrawRectangle((int)(listX+listW-5),(int)sbY,4,(int)sbH,{80,130,200,255});
    }

    // Empty state
    if (lines.size() <= 2) {
        const char* msg = "Pemain ini belum memiliki properti.";
        int mw = MeasureText(msg,13);
        DrawText(msg,(int)(listX+listW/2-mw/2),(int)(listY+listH/2-8),13,{130,130,160,255});
    }
}