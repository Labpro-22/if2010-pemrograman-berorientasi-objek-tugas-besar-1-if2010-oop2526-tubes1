#include "GameScreen.hpp"
#include "../../core/GameMaster/GameMaster.hpp"
#include "../../core/Card/SkillCard.hpp"
#include "../../core/Card/TeleportCard.hpp"
#include "../../core/Card/LassoCard.hpp"
#include "../../core/Card/DemolitionCard.hpp"
#include "../../core/Player/Player.hpp"
#include "../../core/Property/Property.hpp"
#include "../../core/Commands/GunakanKartuKemampuanCommand.hpp"
#include <exception>

static void drawWrappedText(
    const std::string &text,
    int x,
    int y,
    int maxWidth,
    int fontSize,
    Color color)
{
    std::string line;
    std::string word;
    int curY = y;

    for (size_t i = 0; i <= text.size(); ++i)
    {
        char c = (i < text.size()) ? text[i] : ' ';

        if (c == ' ' || i == text.size())
        {
            if (!word.empty())
            {
                std::string testLine = line.empty() ? word : line + " " + word;

                if (MeasureText(testLine.c_str(), fontSize) > maxWidth && !line.empty())
                {
                    DrawText(line.c_str(), x, curY, fontSize, color);
                    curY += fontSize + 6;
                    line = word;
                }
                else
                {
                    line = testLine;
                }

                word.clear();
            }
        }
        else
        {
            word += c;
        }
    }

    if (!line.empty())
    {
        DrawText(line.c_str(), x, curY, fontSize, color);
    }
}

static std::string buildSkillCardSuccessMessage(SkillCard *card)
{
    if (card == nullptr)
        return "Kartu kemampuan berhasil digunakan.";

    return card->successMessage();
}

void GameScreen::triggerSkillCardDialog()
{
    skillCardDialog.visible = true;
    skillCardDialog.hoveredIdx = -1;
    skillCardDialog.awaitingTeleportTile = false;
    skillCardDialog.awaitingLassoTarget = false;
    skillCardDialog.selectedCardIdx = -1;
}

static bool discardUsedSkillCard(GameMaster *gm, Player *player, int idx)
{
    if (gm == nullptr || player == nullptr)
        return false;

    if (idx < 0 || idx >= static_cast<int>(player->getHand().size()))
        return false;

    GameState &gs = gm->getState();

    SkillCard *removed = player->discardSkillCard(idx);

    if (removed != nullptr && gs.getSkillDeck() != nullptr)
    {
        gs.getSkillDeck()->discard(removed);
    }

    gs.setHasUsedCard(true);

    return true;
}

void GameScreen::drawDemolitionTargetDialog()
{
    if (!demolitionTargetDialog.visible)
        return;

    if (!isRealMode() || !guiManager || !guiManager->getGameMaster())
        return;

    GameMaster *gm = guiManager->getGameMaster();
    GameState &gs = gm->getState();
    Player *cur = gs.getCurrPlayer();

    if (cur == nullptr)
        return;

    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, {0, 0, 0, 150});

    const float PW = 560.0f;
    const float PH = 380.0f;
    const float px = SCREEN_W / 2.0f - PW / 2.0f;
    const float py = SCREEN_H / 2.0f - PH / 2.0f;

    DrawRectangleRounded({px, py, PW, PH}, 0.04f, 8, {18, 20, 34, 255});
    DrawRectangleRoundedLines({px, py, PW, PH}, 0.04f, 8, {90, 95, 150, 255});
    DrawText("PILIH PROPERTI TARGET", (int)px + 20, (int)py + 20, 16, WHITE);

    int rowIndex = 0;

    // SESUAIKAN bagian ini dengan struktur project kamu.
    // Contoh kalau Player punya getProperties():
    for (Player *owner : gs.getPlayers())
    {
        if (owner == nullptr || owner == cur || owner->getStatus() == PlayerStatus::BANKRUPT)
            continue;

        for (Property *prop : owner->getProperties())
        {
            if (prop == nullptr)
                continue;

            Rectangle row = {px + 24, py + 70 + rowIndex * 48.0f, PW - 48, 38};
            bool hover = CheckCollisionPointRec(GetMousePosition(), row);

            DrawRectangleRounded(row, 0.08f, 8, hover ? Color{55, 65, 105, 255} : Color{32, 35, 54, 255});

            std::string label = prop->getName() + " (" + prop->getCode() + ") - milik " + owner->getUsername();
            DrawText(label.c_str(), (int)row.x + 14, (int)row.y + 11, 13, WHITE);

            if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                int idx = demolitionTargetDialog.selectedCardIdx;
                const auto &hand = cur->getHand();

                if (idx < 0 || idx >= static_cast<int>(hand.size()))
                {
                    demolitionTargetDialog.errorMsg = "Index DemolitionCard tidak valid.";
                    return;
                }

                DemolitionCard *demoCard = dynamic_cast<DemolitionCard *>(hand[idx]);

                if (demoCard == nullptr)
                {
                    demolitionTargetDialog.errorMsg = "Kartu yang dipilih bukan DemolitionCard.";
                    return;
                }

                try
                {
                    demoCard->setTargetProperty(prop);

                    GunakanKartuKemampuanCommand cmd(idx);
                    cmd.execute(*gm);

                    demolitionTargetDialog.visible = false;
                    demolitionTargetDialog.selectedCardIdx = -1;
                    demolitionTargetDialog.errorMsg.clear();

                    skillCardResultDialog.visible = true;
                    skillCardResultDialog.title = "DemolitionCard";
                    skillCardResultDialog.message =
                        "DemolitionCard berhasil digunakan pada " + prop->getName() + ".";
                }
                catch (const std::exception &e)
                {
                    demolitionTargetDialog.errorMsg = e.what();
                }

                return;
            }

            rowIndex++;
        }
    }

    if (rowIndex == 0)
    {
        DrawText("Tidak ada properti lawan yang valid.", (int)px + 24, (int)py + 80, 13, {220, 220, 220, 255});
    }
}

void GameScreen::drawSkillCardDialog()
{
    if (!skillCardDialog.visible)
        return;

    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, {0, 0, 0, 170});

    const float PW = 500.f;
    const float PH = 400.f;
    const float px = SCREEN_W / 2.f - PW / 2.f;
    const float py = SCREEN_H / 2.f - PH / 2.f;

    DrawRectangle((int)px, (int)py, (int)PW, (int)PH, {22, 24, 38, 255});
    DrawRectangleLinesEx({px, py, PW, PH}, 1.5f, {80, 80, 130, 255});

    DrawRectangle((int)px, (int)py, (int)PW, 44, {32, 34, 52, 255});
    DrawText("GUNAKAN KARTU SKILL", (int)(px + 16), (int)(py + 14), 16, {180, 180, 230, 255});

    Rectangle xBtn = {px + PW - 36, py + 8, 28, 28};
    bool xHov = CheckCollisionPointRec(GetMousePosition(), xBtn);

    DrawRectangleRec(xBtn, xHov ? Color{180, 60, 60, 255} : Color{110, 40, 40, 255});
    DrawText("X", (int)(xBtn.x + 9), (int)(xBtn.y + 8), 12, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && xHov)
    {
        skillCardDialog.visible = false;
        return;
    }

    if (!isRealMode() || !guiManager || !guiManager->getGameMaster())
    {
        DrawText("Game tidak dalam mode REAL.", (int)px + 20, (int)py + 80, 14, {150, 150, 150, 255});
        return;
    }

    GameMaster *gm = guiManager->getGameMaster();
    Player *cur = gm->getState().getCurrPlayer();

    if (!cur)
    {
        DrawText("Tidak ada pemain aktif.", (int)px + 20, (int)py + 80, 14, {150, 150, 150, 255});
        return;
    }

    const auto &hand = cur->getHand();

    if (hand.empty())
    {
        DrawText("Kamu tidak memiliki kartu skill.", (int)px + 20, (int)py + 80, 14, {150, 150, 150, 255});
        return;
    }

    const float listY = py + 64.f;

    for (int i = 0; i < static_cast<int>(hand.size()); ++i)
    {
        float rowY = listY + i * 48.f;

        if (rowY + 42 > py + PH - 16)
            break;

        Rectangle row = {px + 16, rowY, PW - 32, 42};
        bool hover = CheckCollisionPointRec(GetMousePosition(), row);

        DrawRectangleRec(row, hover ? Color{45, 50, 75, 255} : Color{30, 32, 48, 255});
        DrawRectangleLinesEx(row, 1.0f, {75, 80, 115, 255});

        SkillCard *card = hand[i];
        std::string type = card ? card->getType() : "UnknownCard";

        DrawText(type.c_str(), (int)row.x + 12, (int)row.y + 13, 14, WHITE);

        if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            SkillCard *selectedCard = card;

            if (selectedCard == nullptr)
                return;

            std::string type = selectedCard->getType();

            if (type == "TeleportCard")
            {
                skillCardDialog.awaitingTeleportTile = true;
                skillCardDialog.selectedCardIdx = i;
                skillCardDialog.visible = false;

                skillTargetHint.visible = true;
                skillTargetHint.message = selectedCard->successMessage();

                return;
            }

            if (type == "LassoCard")
            {
                skillCardDialog.visible = false;

                lassoTargetDialog.visible = true;
                lassoTargetDialog.selectedCardIdx = i;
                lassoTargetDialog.errorMsg.clear();

                return;
            }

            if (type == "DemolitionCard")
            {
                skillCardDialog.visible = false;

                demolitionTargetDialog.visible = true;
                demolitionTargetDialog.selectedCardIdx = i;
                demolitionTargetDialog.errorMsg.clear();

                return;
            }

            try
            {
                std::string successTitle = selectedCard->getType();
                std::string successMessage = selectedCard->successMessage();

                GunakanKartuKemampuanCommand cmd(i);
                cmd.execute(*gm);

                skillCardDialog.visible = false;

                skillCardResultDialog.visible = true;
                skillCardResultDialog.title = successTitle;
                skillCardResultDialog.message = successMessage;
            }
            catch (const std::exception &e)
            {
                skillCardDialog.visible = false;

                skillCardResultDialog.visible = true;
                skillCardResultDialog.title = "Gagal Menggunakan Kartu";
                skillCardResultDialog.message = e.what();
            }

            return;
        }
    }
}

void GameScreen::drawSkillCardResultDialog()
{
    if (!skillCardResultDialog.visible)
        return;

    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, {0, 0, 0, 170});

    const float PW = 620.0f;
    const float PH = 300.0f;
    const float px = SCREEN_W / 2.0f - PW / 2.0f;
    const float py = SCREEN_H / 2.0f - PH / 2.0f;

    DrawRectangleRounded({px, py, PW, PH}, 0.04f, 8, {18, 20, 34, 255});
    DrawRectangleRoundedLines({px, py, PW, PH}, 0.04f, 8, {90, 95, 150, 255});

    DrawRectangleRounded({px, py, PW, 54}, 0.04f, 8, {34, 36, 58, 255});
    DrawText("KARTU KEMAMPUAN", (int)(px + 22), (int)(py + 18), 16, WHITE);

    DrawText(
        skillCardResultDialog.title.c_str(),
        (int)(px + 28),
        (int)(py + 82),
        22,
        {255, 230, 150, 255});

    Rectangle msgBox = {px + 28, py + 122, PW - 56, 92};
    DrawRectangleRounded(msgBox, 0.04f, 8, {30, 33, 52, 255});
    DrawRectangleRoundedLines(msgBox, 0.04f, 8, {75, 80, 120, 255});

    drawWrappedText(
        skillCardResultDialog.message,
        (int)(msgBox.x + 14),
        (int)(msgBox.y + 16),
        (int)(msgBox.width - 28),
        13,
        {230, 230, 235, 255});

    Rectangle okBtn = {px + PW / 2.0f - 60.0f, py + PH - 62.0f, 120.0f, 38.0f};
    bool hover = CheckCollisionPointRec(GetMousePosition(), okBtn);

    DrawRectangleRounded(okBtn, 0.12f, 8, hover ? Color{82, 98, 155, 255} : Color{58, 68, 115, 255});
    DrawRectangleRoundedLines(okBtn, 0.12f, 8, {120, 130, 180, 255});
    DrawText("OK", (int)(okBtn.x + 47), (int)(okBtn.y + 11), 14, WHITE);

    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        skillCardResultDialog.visible = false;
        skillCardResultDialog.title.clear();
        skillCardResultDialog.message.clear();
    }
}

void GameScreen::handleTeleportTargetClick(int selectedTile)
{
    if (!skillCardDialog.awaitingTeleportTile)
        return;

    if (!isRealMode() || !guiManager || !guiManager->getGameMaster())
        return;

    GameMaster *gm = guiManager->getGameMaster();
    GameState &gs = gm->getState();
    Player *cur = gs.getCurrPlayer();

    if (cur == nullptr || selectedTile < 0)
        return;

    int idx = skillCardDialog.selectedCardIdx;
    const auto &hand = cur->getHand();

    if (idx < 0 || idx >= static_cast<int>(hand.size()))
    {
        skillCardResultDialog.visible = true;
        skillCardResultDialog.title = "Gagal Menggunakan Kartu";
        skillCardResultDialog.message = "Index TeleportCard tidak valid.";
        return;
    }

    TeleportCard *teleportCard = dynamic_cast<TeleportCard *>(hand[idx]);

    if (teleportCard == nullptr)
    {
        skillCardResultDialog.visible = true;
        skillCardResultDialog.title = "Gagal Menggunakan Kartu";
        skillCardResultDialog.message = "Kartu yang dipilih bukan TeleportCard.";
        return;
    }

    try
    {
        teleportCard->setTargetPosition(selectedTile, gs);

        GunakanKartuKemampuanCommand cmd(idx);
        cmd.execute(*gm);

        skillCardDialog.awaitingTeleportTile = false;
        skillCardDialog.selectedCardIdx = -1;
        skillTargetHint.visible = false;

        skillCardResultDialog.visible = true;
        skillCardResultDialog.title = "TeleportCard";
        skillCardResultDialog.message = "TeleportCard berhasil digunakan. Bidak dipindahkan ke petak tujuan.";
    }
    catch (const std::exception &e)
    {
        skillCardDialog.awaitingTeleportTile = false;
        skillCardDialog.selectedCardIdx = -1;
        skillTargetHint.visible = false;

        skillCardResultDialog.visible = true;
        skillCardResultDialog.title = "Gagal Menggunakan Kartu";
        skillCardResultDialog.message = e.what();
    }
}

void GameScreen::drawSkillTargetHint()
{
    if (!skillTargetHint.visible)
        return;

    Rectangle bar = {340, 18, SCREEN_W - 680.0f, 44};

    DrawRectangleRounded(bar, 0.12f, 8, {22, 24, 38, 235});
    DrawRectangleRoundedLines(bar, 0.12f, 8, {120, 130, 190, 255});

    drawWrappedText(
        skillTargetHint.message,
        (int)(bar.x + 18),
        (int)(bar.y + 13),
        (int)(bar.width - 36),
        13,
        {245, 245, 245, 255});
}

void GameScreen::drawLassoTargetDialog()
{
    if (!lassoTargetDialog.visible)
        return;

    if (!isRealMode() || !guiManager || !guiManager->getGameMaster())
        return;

    GameMaster *gm = guiManager->getGameMaster();
    GameState &gs = gm->getState();
    Player *cur = gs.getCurrPlayer();

    if (cur == nullptr)
        return;

    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, {0, 0, 0, 150});

    const float PW = 460.0f;
    const float PH = 320.0f;
    const float px = SCREEN_W / 2.0f - PW / 2.0f;
    const float py = SCREEN_H / 2.0f - PH / 2.0f;

    DrawRectangleRounded({px, py, PW, PH}, 0.04f, 8, {18, 20, 34, 255});
    DrawRectangleRoundedLines({px, py, PW, PH}, 0.04f, 8, {90, 95, 150, 255});
    DrawText("PILIH TARGET LASSO", (int)px + 20, (int)py + 20, 16, WHITE);

    int rowIndex = 0;

    for (Player *p : gs.getPlayers())
    {
        if (p == nullptr || p == cur || p->getStatus() == PlayerStatus::BANKRUPT)
            continue;

        Rectangle row = {px + 24, py + 70 + rowIndex * 48.0f, PW - 48, 38};
        bool hover = CheckCollisionPointRec(GetMousePosition(), row);

        DrawRectangleRounded(row, 0.08f, 8, hover ? Color{55, 65, 105, 255} : Color{32, 35, 54, 255});

        std::string label = p->getUsername() + " - posisi " + std::to_string(p->getPosition());
        DrawText(label.c_str(), (int)row.x + 14, (int)row.y + 11, 13, WHITE);

        if (hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            int idx = lassoTargetDialog.selectedCardIdx;
            const auto &hand = cur->getHand();

            if (idx < 0 || idx >= static_cast<int>(hand.size()))
            {
                lassoTargetDialog.errorMsg = "Index LassoCard tidak valid.";
                return;
            }

            LassoCard *lassoCard = dynamic_cast<LassoCard *>(hand[idx]);

            if (lassoCard == nullptr)
            {
                lassoTargetDialog.errorMsg = "Kartu yang dipilih bukan LassoCard.";
                return;
            }

            try
            {
                lassoCard->setTargetPlayer(p);

                GunakanKartuKemampuanCommand cmd(idx);
                cmd.execute(*gm);

                lassoTargetDialog.visible = false;
                lassoTargetDialog.selectedCardIdx = -1;
                lassoTargetDialog.errorMsg.clear();

                skillCardResultDialog.visible = true;
                skillCardResultDialog.title = "LassoCard";
                skillCardResultDialog.message =
                    "LassoCard berhasil digunakan. " + p->getUsername() +
                    " ditarik ke petak Anda.";
            }
            catch (const std::exception &e)
            {
                lassoTargetDialog.errorMsg = e.what();
            }

            return;
        }

        rowIndex++;
    }

    if (rowIndex == 0)
    {
        DrawText("Tidak ada pemain lawan yang valid.", (int)px + 24, (int)py + 80, 13, {220, 220, 220, 255});
    }
    if (!lassoTargetDialog.errorMsg.empty())
    {
        DrawText(
            lassoTargetDialog.errorMsg.c_str(),
            (int)px + 24,
            (int)(py + PH - 42),
            12,
            {255, 120, 120, 255});
    }
}
