#include "GameScreen.hpp"
#include "../../core/GameMaster/GameMaster.hpp"
#include "../../core/Card/SkillCard.hpp"
#include "../../core/Player/Player.hpp"

void GameScreen::triggerSkillCardDialog()
{
    skillCardDialog.visible = true;
    skillCardDialog.hoveredIdx = -1;
    skillCardDialog.awaitingTeleportTile = false;
    skillCardDialog.awaitingLassoTarget = false;
    skillCardDialog.selectedCardIdx = -1;
}

void GameScreen::drawSkillCardDialog()
{
    if (!skillCardDialog.visible)
        return;

    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, {0, 0, 0, 170});

    float PW = 500.f, PH = 400.f;
    float px = SCREEN_W / 2.f - PW / 2.f;
    float py = SCREEN_H / 2.f - PH / 2.f;

    DrawRectangle((int)px, (int)py, (int)PW, (int)PH, {22, 24, 38, 255});
    DrawRectangleLinesEx({px, py, PW, PH}, 1.5f, {80, 80, 130, 255});
    DrawRectangle((int)px, (int)py, (int)PW, 44, {32, 34, 52, 255});
    DrawText("GUNAKAN KARTU SKILL", (int)(px + 16), (int)(py + 14), 16, {180, 180, 230, 255});

    Rectangle xBtn = {px + PW - 36, py + 8, 28, 28};
    bool xHov = CheckCollisionPointRec(GetMousePosition(), xBtn);
    DrawRectangleRec(xBtn, xHov ? Color{180, 60, 60, 255} : Color{110, 40, 40, 255});
    DrawText("X", (int)(xBtn.x + 9), (int)(xBtn.y + 8), 12, WHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && xHov)
        skillCardDialog.visible = false;

    if (!isRealMode() || !guiManager || !guiManager->getGameMaster())
    {
        DrawText("Game tidak dalam mode REAL.", (int)px + 20, (int)py + 80, 14, {150, 150, 150, 255});
        return;
    }

    GameMaster *gm = guiManager->getGameMaster();
    Player *cur = gm->getState().getCurrPlayer();

    if (!cur || cur->getHand().empty())
    {
        DrawText("Kamu tidak memiliki kartu skill.", (int)px + 20, (int)py + 80, 14, {150, 150, 150, 255});
        return;
    }

    // Jika sedang memilih target (Teleport/Lasso)
    if (skillCardDialog.awaitingTeleportTile || skillCardDialog.awaitingLassoTarget)
    {
        DrawText("Pilih target di papan dan klik OK.", (int)px + 20, (int)py + 80, 14, WHITE);

        // Tombol OK
        Rectangle okBtn = {px + PW / 2 - 50, py + PH - 50, 100, 30};
        bool okHov = CheckCollisionPointRec(GetMousePosition(), okBtn);
        DrawRectangleRec(okBtn, okHov ? Color{60, 180, 90, 255} : Color{40, 120, 60, 255});
        DrawText("OK", (int)okBtn.x + 40, (int)okBtn.y + 8, 14, WHITE);

        if (okHov && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Asumsi `selectedTile` terupdate oleh input papan di main loop
            if (skillCardDialog.awaitingTeleportTile && selectedTile >= 0)
            {
                // Di sini bisa menggunakan setter untuk kartu khusus (contoh dummy teleport ke target)
                gm->useSkillCard(cur, cur->getHand()[skillCardDialog.selectedCardIdx], gm->getState());
                gm->teleportPlayer(cur, selectedTile, true);
                cur->discardSkillCard(skillCardDialog.selectedCardIdx);
                skillCardDialog.visible = false;
            }
            else if (skillCardDialog.awaitingLassoTarget)
            {
                // Sederhana: gunakan kartu, discard, efek diterapkan di backend.
                // Untuk lasso, target bisa diset.
                gm->useSkillCard(cur, cur->getHand()[skillCardDialog.selectedCardIdx], gm->getState());
                cur->discardSkillCard(skillCardDialog.selectedCardIdx);
                skillCardDialog.visible = false;
            }
        }
        return;
    }

    float listY = py + 60;
    auto &hand = cur->getHand();
    for (size_t i = 0; i < hand.size(); ++i)
    {
        float rowY = listY + i * 40;
        if (rowY + 40 > py + PH)
            break;
        Rectangle row = {px + 10, rowY, PW - 20, 36};
        bool hover = CheckCollisionPointRec(GetMousePosition(), row);
        DrawRectangleRec(row, hover ? Color{40, 45, 65, 255} : Color{30, 32, 48, 255});

        std::string txt = hand[i]->getType();
        DrawText(txt.c_str(), (int)row.x + 10, (int)row.y + 10, 14, WHITE);

        if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            skillCardDialog.selectedCardIdx = i;
            if (txt == "TeleportCard")
            {
                skillCardDialog.awaitingTeleportTile = true;
            }
            else if (txt == "LassoCard" || txt == "DemolitionCard")
            {
                skillCardDialog.awaitingLassoTarget = true;
            }
            else
            {
                gm->useSkillCard(cur, hand[i], gm->getState());
                skillCardDialog.visible = false;
            }
        }
    }
}
