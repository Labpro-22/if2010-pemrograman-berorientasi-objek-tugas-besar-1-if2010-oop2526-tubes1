#include "GameScreen.hpp"
#include "../../../lib/raylib/include/raylib.h"
#include "../../core/GameMaster/GameMaster.hpp"
#include "../../core/GameState/GameState.hpp"
#include "../../core/Player/Player.hpp"
#include "../../core/Commands/CetakPropertiCommand.hpp"

#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

static std::vector<std::string> splitLines(const std::string &text)
{
    std::vector<std::string> lines;
    std::istringstream iss(text);
    std::string line;

    while (std::getline(iss, line))
        lines.push_back(line);

    if (lines.empty())
        lines.push_back("");

    return lines;
}

static bool isEmptyPropertyContent(const std::vector<std::string> &lines)
{
    if (lines.empty())
        return true;

    if (lines.size() == 1 && lines[0].empty())
        return true;

    for (const std::string &line : lines)
    {
        if (line.find("belum memiliki properti") != std::string::npos)
            return true;

        if (line.find("Belum memiliki properti") != std::string::npos)
            return true;
    }

    return false;
}

void GameScreen::triggerCetakPropertiDialog(int playerIdx)
{
    cetakPropertiDialog.scrollY = 0.f;
    cetakPropertiDialog.playerIdx = playerIdx;
    cetakPropertiDialog.visible = true;

    if (isRealMode())
    {
        if (!guiManager || !guiManager->getGameMaster())
        {
            cetakPropertiDialog.content = "GameMaster tidak tersedia.\n";
            cetakPropertiDialog.playerName = "?";
            cetakPropertiDialog.playerIdx = -1;
            return;
        }

        GameMaster *gm = guiManager->getGameMaster();
        GameState &gs = gm->getState();
        Player *p = gs.getCurrPlayer();

        if (!p)
        {
            cetakPropertiDialog.content = "Tidak ada pemain aktif.\n";
            cetakPropertiDialog.playerName = "?";
            cetakPropertiDialog.playerIdx = -1;
            return;
        }

        cetakPropertiDialog.playerName = p->getUsername();

        const auto &players = gs.getPlayers();
        int currentIdx = -1;

        for (int i = 0; i < static_cast<int>(players.size()); i++)
        {
            if (players[i] == p)
            {
                currentIdx = i;
                break;
            }
        }

        cetakPropertiDialog.playerIdx = currentIdx;

        // INI BAGIAN PENTING:
        // Isi tengah dialog diambil dari CetakPropertiCommand.
        CetakPropertiCommand cmd;
        cetakPropertiDialog.content = cmd.getOutput(*gm);

        return;
    }

    if (playerIdx < 0 || playerIdx >= static_cast<int>(gameState.players.size()))
    {
        cetakPropertiDialog.playerName = "?";
        cetakPropertiDialog.content = "Pemain tidak valid.\n";
        cetakPropertiDialog.playerIdx = -1;
        return;
    }

    cetakPropertiDialog.playerName = gameState.players[playerIdx].username;

    std::ostringstream oss;
    bool found = false;

    for (int i = 0; i < static_cast<int>(gameState.properties.size()); i++)
    {
        if (gameState.properties[i].owner == playerIdx)
        {
            found = true;
            auto &mp = gameState.properties[i];

            oss << "[" << mp.code << "] " << mp.name << "\n"
                << "  Harga    : M" << mp.price << "\n"
                << "  Gadai    : M" << mp.mortgageVal << "\n"
                << "  Bangunan : "
                << (mp.buildings == 5  ? "Hotel"
                    : mp.buildings > 0 ? std::to_string(mp.buildings) + " rumah"
                                       : "-")
                << "\n\n";
        }
    }

    if (!found)
        oss << "Kamu belum memiliki properti apapun.\n";

    cetakPropertiDialog.content = oss.str();
}

void GameScreen::drawCetakPropertiDialog()
{
    if (!cetakPropertiDialog.visible)
        return;

    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, {0, 0, 0, 160});

    constexpr float PW = 580.f;
    constexpr float PH = 560.f;

    float px = SCREEN_W / 2.f - PW / 2.f;
    float py = SCREEN_H / 2.f - PH / 2.f;

    DrawRectangle((int)px, (int)py, (int)PW, (int)PH, {18, 20, 30, 255});
    DrawRectangleLinesEx({px, py, PW, PH}, 2.f, {80, 130, 200, 255});

    int pidx = cetakPropertiDialog.playerIdx;

    Color hdrCol = (pidx >= 0 && pidx < 4)
                       ? Color{(unsigned char)(playerColors[pidx].r / 2),
                               (unsigned char)(playerColors[pidx].g / 2),
                               (unsigned char)(playerColors[pidx].b / 2),
                               255}
                       : Color{30, 50, 80, 255};

    DrawRectangle((int)px, (int)py, (int)PW, 48, hdrCol);

    if (pidx >= 0 && pidx < 4)
        DrawRectangle((int)px, (int)py, 6, 48, playerColors[pidx]);

    std::string title = "PROPERTI - " + cetakPropertiDialog.playerName;
    int tw = MeasureText(title.c_str(), 14);
    DrawText(title.c_str(), (int)(px + PW / 2 - tw / 2), (int)(py + 16), 14, WHITE);

    Rectangle xBtn = {px + PW - 36, py + 10, 28, 28};
    bool xHov = CheckCollisionPointRec(GetMousePosition(), xBtn);

    DrawRectangleRec(xBtn, xHov ? Color{180, 60, 60, 255} : Color{110, 40, 40, 255});
    DrawText("X", (int)(xBtn.x + 9), (int)(xBtn.y + 8), 12, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && xHov)
    {
        cetakPropertiDialog.visible = false;
        return;
    }

    float listX = px + 16;
    float listY = py + 62;
    float listW = PW - 32;
    float listH = PH - 84;

    std::vector<std::string> lines = splitLines(cetakPropertiDialog.content);
    bool emptyState = isEmptyPropertyContent(lines);

    constexpr float ROW_H = 18.f;
    float totalH = lines.size() * ROW_H;

    Rectangle listArea = {listX, listY, listW, listH};

    if (CheckCollisionPointRec(GetMousePosition(), listArea))
    {
        cetakPropertiDialog.scrollY -= GetMouseWheelMove() * 36.f;

        float maxScroll = std::max(0.f, totalH - listH);

        cetakPropertiDialog.scrollY =
            std::max(0.f, std::min(cetakPropertiDialog.scrollY, maxScroll));
    }

    BeginScissorMode((int)listX, (int)listY, (int)listW, (int)listH);

    // Kalau command menghasilkan "Kamu belum memiliki properti apapun.",
    // GUI menampilkannya sebagai empty state di tengah, bukan sebagai teks biasa di atas.
    if (emptyState)
    {
        const char *msg = "Kamu belum memiliki properti apapun.";
        int mw = MeasureText(msg, 13);

        DrawText(
            msg,
            (int)(listX + listW / 2 - mw / 2),
            (int)(listY + listH / 2 - 8),
            13,
            {150, 150, 180, 255});
    }
    else
    {
        for (int i = 0; i < static_cast<int>(lines.size()); i++)
        {
            float ry = listY + i * ROW_H - cetakPropertiDialog.scrollY;

            if (ry + ROW_H < listY || ry > listY + listH)
                continue;

            Color c;

            if (lines[i].find("===") != std::string::npos)
            {
                c = (pidx >= 0 && pidx < 4)
                        ? playerColors[pidx]
                        : Color{200, 200, 100, 255};
            }
            else if (lines[i].find('[') != std::string::npos)
            {
                c = {220, 220, 255, 255};
            }
            else if (lines[i].find("Hotel") != std::string::npos)
            {
                c = {255, 160, 80, 255};
            }
            else if (lines[i].find("rumah") != std::string::npos)
            {
                c = {150, 220, 150, 255};
            }
            else
            {
                c = {180, 180, 200, 255};
            }

            DrawText(lines[i].c_str(), (int)(listX + 4), (int)(ry + 2), 11, c);
        }
    }

    EndScissorMode();

    if (!emptyState && totalH > listH)
    {
        float sbTrackH = listH;
        float sbH = listH * (listH / totalH);
        float maxScroll = std::max(1.f, totalH - listH);
        float sbY = listY + (cetakPropertiDialog.scrollY / maxScroll) * (sbTrackH - sbH);

        DrawRectangle((int)(listX + listW - 5), (int)listY, 4, (int)listH, {40, 42, 58, 255});
        DrawRectangle((int)(listX + listW - 5), (int)sbY, 4, (int)sbH, {80, 130, 200, 255});
    }
}