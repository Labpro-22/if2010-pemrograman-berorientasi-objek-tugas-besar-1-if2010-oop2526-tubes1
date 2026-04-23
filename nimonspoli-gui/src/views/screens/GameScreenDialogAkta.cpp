#include "GameScreen.hpp"
#include "../../../lib/raylib/include/raylib.h"
#include "../../core/GameMaster/GameMaster.hpp"
#include "../../core/Board/Board.hpp"
#include "../../core/Property/Property.hpp"
#include <algorithm>
#include <sstream>
#include <string>

extern const TileDef TILE_DEFS[40];

// ─── Trigger — hanya buka dialog, content kosong dulu ────────────────────────
void GameScreen::triggerAktaDialog()
{
    aktaDialog.visible   = true;
    aktaDialog.inputCode = "";
    aktaDialog.content   = "";
    aktaDialog.propName  = "";
    aktaDialog.scrollY   = 0.f;
    aktaDialog.inputFocused = true;
    aktaDialog.errorMsg  = "";
}

// ─── Cari properti berdasarkan kode, isi content ─────────────────────────────
void GameScreen::searchAndFillAkta()
{
    aktaDialog.errorMsg = "";
    aktaDialog.content  = "";
    aktaDialog.propName = "";
    aktaDialog.scrollY  = 0.f;

    std::string code = aktaDialog.inputCode;
    // Uppercase
    std::transform(code.begin(), code.end(), code.begin(), ::toupper);

    if (code.empty()) { aktaDialog.errorMsg = "Masukkan kode petak!"; return; }

    // Mode real — cari di Board
    if (guiManager && guiManager->getGameMaster()) {
        Board* bd = guiManager->getGameMaster()->getState().getBoard();
        if (!bd) { aktaDialog.errorMsg = "Board tidak tersedia."; return; }

        // Loop tile 1-40 cari yang kodenya cocok
        Property* found = nullptr;
        for (int i = 1; i <= 40; i++) {
            Tile* tile = bd->getTile(i);
            if (!tile) continue;
            PropertyTile* pt = dynamic_cast<PropertyTile*>(tile);
            if (!pt || !pt->getProperty()) continue;
            if (pt->getProperty()->getCode() == code) {
                found = pt->getProperty();
                break;
            }
        }

        if (!found) {
            aktaDialog.errorMsg = "Petak \"" + code + "\" tidak ditemukan atau bukan properti.";
            return;
        }
        aktaDialog.propName = found->getName().empty() ? found->getCode() : found->getName();
        aktaDialog.content  = found->cetakAkta();
        return;
    }

    // Mode mock — cari di gameState.properties
    for (auto& mp : gameState.properties) {
        std::string mpCode = mp.code;
        std::transform(mpCode.begin(), mpCode.end(), mpCode.begin(), ::toupper);
        if (mpCode == code) {
            aktaDialog.propName = mp.name.empty() ? mp.code : mp.name;
            std::ostringstream oss;
            oss << "=== AKTA KEPEMILIKAN ===\n"
                << "[" << mp.colorGroup << "] " << mp.name << " (" << mp.code << ")\n"
                << "================================\n"
                << "Harga Beli        : M" << mp.price      << "\n"
                << "Nilai Gadai       : M" << mp.mortgageVal << "\n"
                << "--------------------------------\n"
                << "Sewa (tanah)      : M" << mp.rentL0     << "\n"
                << "Sewa (1 rumah)    : M" << mp.rentL1     << "\n"
                << "Sewa (2 rumah)    : M" << mp.rentL2     << "\n"
                << "Sewa (3 rumah)    : M" << mp.rentL3     << "\n"
                << "Sewa (4 rumah)    : M" << mp.rentL4     << "\n"
                << "Sewa (hotel)      : M" << mp.rentL5     << "\n"
                << "--------------------------------\n"
                << "Harga Rumah       : M" << mp.houseUpg   << "\n"
                << "Harga Hotel       : M" << mp.hotelUpg   << "\n"
                << "================================\n"
                << "Status: " << (mp.owner < 0 ? "BANK"
                                : "OWNED (" + gameState.players[mp.owner].username + ")")
                << "\n";
            aktaDialog.content = oss.str();
            return;
        }
    }
    aktaDialog.errorMsg = "Petak \"" + code + "\" tidak ditemukan atau bukan properti.";
}

// ─── Render ──────────────────────────────────────────────────────────────────
void GameScreen::drawAktaDialog()
{
    if (!aktaDialog.visible) return;

    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, {0,0,0,160});

    constexpr float PW = 560.f, PH = 560.f;
    float px = SCREEN_W/2.f - PW/2.f;
    float py = SCREEN_H/2.f - PH/2.f;

    DrawRectangle((int)px,(int)py,(int)PW,(int)PH,{18,20,30,255});
    DrawRectangleLinesEx({px,py,PW,PH},2.f,{180,150,80,255});

    // Header
    DrawRectangle((int)px,(int)py,(int)PW,48,{80,65,15,255});
    const char* title = "CETAK AKTA PROPERTI";
    int tw = MeasureText(title,14);
    DrawText(title,(int)(px+PW/2-tw/2),(int)(py+16),14,WHITE);

    // Tombol X
    Rectangle xBtn = {px+PW-36,py+10,28,28};
    bool xHov = CheckCollisionPointRec(GetMousePosition(),xBtn);
    DrawRectangleRec(xBtn,xHov?Color{180,60,60,255}:Color{110,40,40,255});
    DrawText("X",(int)(xBtn.x+9),(int)(xBtn.y+8),12,WHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && xHov) {
        aktaDialog.visible = false; return;
    }
    if (IsKeyPressed(KEY_ESCAPE)) { aktaDialog.visible = false; return; }

    // ── Input area ────────────────────────────────────────────────────────
    float iy = py + 60;
    DrawText("Kode Petak:",(int)(px+16),(int)(iy+8),12,{180,180,210,255});

    Rectangle inputBox = {px+120, iy, PW-200, 36};
    bool inputHov = CheckCollisionPointRec(GetMousePosition(),inputBox);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        aktaDialog.inputFocused = inputHov;

    DrawRectangleRec(inputBox, aktaDialog.inputFocused
                    ? Color{35,38,58,255} : Color{25,27,42,255});
    DrawRectangleLinesEx(inputBox,1.5f, aktaDialog.inputFocused
                    ? Color{180,150,60,255} : Color{80,80,110,255});

    // Keyboard input
    if (aktaDialog.inputFocused) {
        if (IsKeyPressed(KEY_BACKSPACE) && !aktaDialog.inputCode.empty())
            aktaDialog.inputCode.pop_back();
        int ch = GetCharPressed();
        while (ch > 0) {
            if (aktaDialog.inputCode.size() < 6)   // kode max 6 char
                aktaDialog.inputCode += (char)::toupper(ch);
            ch = GetCharPressed();
        }
        if (IsKeyPressed(KEY_ENTER)) {
            searchAndFillAkta();
        }
    }

    // Tampilkan text input + cursor
    std::string disp = aktaDialog.inputCode.empty() ? "contoh: JKT" : aktaDialog.inputCode;
    Color textCol = aktaDialog.inputCode.empty() ? Color{80,80,110,255} : WHITE;
    DrawText(disp.c_str(),(int)(inputBox.x+10),(int)(inputBox.y+10),14,textCol);
    if (aktaDialog.inputFocused && (int)(GetTime()*2)%2==0 && !aktaDialog.inputCode.empty()) {
        int cw = MeasureText(aktaDialog.inputCode.c_str(),14);
        DrawText("|",(int)(inputBox.x+12+cw),(int)(inputBox.y+10),14,{180,150,60,255});
    }

    // Tombol CARI
    Rectangle cariBtn = {px+PW-76, iy, 62, 36};
    bool cariHov = CheckCollisionPointRec(GetMousePosition(),cariBtn);
    DrawRectangleRec(cariBtn, cariHov?Color{60,140,80,255}:Color{40,100,60,255});
    DrawRectangleLinesEx(cariBtn,1,{80,200,110,255});
    int cw2 = MeasureText("CARI",12);
    DrawText("CARI",(int)(cariBtn.x+31-cw2/2),(int)(cariBtn.y+11),12,WHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && cariHov) {
        aktaDialog.inputFocused = false;
        searchAndFillAkta();
    }

    // Error message
    if (!aktaDialog.errorMsg.empty()) {
        DrawText(aktaDialog.errorMsg.c_str(),(int)(px+16),(int)(iy+44),11,{220,80,80,255});
    }

    // ── Content (scrollable) ──────────────────────────────────────────────
    float listY = py + 116;
    float listH = PH - 130;
    float listX = px + 16;
    float listW = PW - 32;

    if (aktaDialog.content.empty() && aktaDialog.errorMsg.empty()) {
        // Hint awal
        DrawText("Masukkan kode petak lalu tekan ENTER atau klik CARI.",
                 (int)(px+16),(int)(listY+listH/2-20),11,{100,100,140,255});
        DrawText("Contoh kode: JKT, BDG, IKN, GBR, PLN",
                 (int)(px+16),(int)(listY+listH/2),11,{80,80,110,255});
        return;
    }

    // Split content jadi baris
    std::vector<std::string> lines;
    std::istringstream iss(aktaDialog.content);
    std::string line;
    while (std::getline(iss, line)) lines.push_back(line);

    constexpr float ROW_H = 17.f;
    float totalH = lines.size() * ROW_H;

    Rectangle listArea = {listX,listY,listW,listH};
    if (CheckCollisionPointRec(GetMousePosition(),listArea)) {
        aktaDialog.scrollY -= GetMouseWheelMove()*36.f;
        aktaDialog.scrollY  = std::max(0.f,std::min(aktaDialog.scrollY,
                              std::max(0.f,totalH-listH)));
    }

    BeginScissorMode((int)listX,(int)listY,(int)listW,(int)listH);
    for (int i = 0; i < (int)lines.size(); i++) {
        float ry = listY + i*ROW_H - aktaDialog.scrollY;
        if (ry+ROW_H < listY || ry > listY+listH) continue;
        Color c = (lines[i].find('=')!=std::string::npos) ? Color{220,190,80,255}
                : (lines[i].find('-')!=std::string::npos && lines[i].size()>4) ? Color{60,60,80,255}
                : (lines[i].find(':')!=std::string::npos) ? Color{200,200,215,255}
                : Color{180,180,200,255};
        DrawText(lines[i].c_str(),(int)(listX+4),(int)(ry+2),11,c);
    }
    EndScissorMode();

    // Scrollbar
    if (totalH > listH) {
        float sbH = listH*(listH/totalH);
        float sbY = listY+(aktaDialog.scrollY/totalH)*listH;
        DrawRectangle((int)(listX+listW-5),(int)listY,4,(int)listH,{40,42,58,255});
        DrawRectangle((int)(listX+listW-5),(int)sbY,4,(int)sbH,{200,160,40,255});
    }
}