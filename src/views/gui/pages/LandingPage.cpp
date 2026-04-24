#include "ui/GUIView.hpp"

#if NIMONSPOLY_ENABLE_RAYLIB
#include <array>
#include <string>

#include "../components/GUIViewDraw.hpp"
#include "PageShared.hpp"
#endif

void GUIView::drawLandingPage() {
#if NIMONSPOLY_ENABLE_RAYLIB
    AssetManager& am = AssetManager::get();
    ClearBackground(RL_BLACK);
    gui::page::drawMenuBackground(am);

    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const float cx = W * 0.5f;
    const float cy = H * 0.5f;

    if (const Texture2D* titleTex = am.texture("assets/bg/Title.png")) {
        const float base = std::min(W, H);
        const float titleW = base * 0.55f;
        const float titleH = titleW * static_cast<float>(titleTex->height) / static_cast<float>(titleTex->width);
        gui::draw::drawSprite(titleTex, Rectangle{
            cx - titleW * 0.5f,
            cy - H * 0.04f - titleH * 0.5f,
            titleW,
            titleH,
        });
    } else {
        gui::draw::drawCenteredText(am, "title", "NIMONS", H * 0.12f, RL_WHITE, cy - H * 0.10f);
        gui::draw::drawCenteredText(am, "title", "POLY", H * 0.12f, RL_WHITE, cy + H * 0.02f);
    }

    const std::array<const char*, 4> items{"New Game", "Load Game", "Credits", "Exit"};
    const float buttonW = std::min(W * 0.24f, 360.f);
    const float buttonH = std::min(H * 0.062f, 64.f);
    const float startY = cy + H * 0.22f;
    const float gap = H * 0.072f;

    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        const Rectangle rect = gui::draw::centeredRect(cx, startY + i * gap, buttonW, buttonH);
        gui::draw::drawMenuButton(am, items[static_cast<size_t>(i)], rect, hoveredItem_ == i, false);
    }
#endif
}

bool GUIView::handleLandingInput() {
#if NIMONSPOLY_ENABLE_RAYLIB
    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const float cx = W * 0.5f;
    const float cy = H * 0.5f;
    const Vector2 mouse = GetMousePosition();

    const float buttonW = std::min(W * 0.24f, 360.f);
    const float buttonH = std::min(H * 0.062f, 64.f);
    const float startY = cy + H * 0.22f;
    const float gap = H * 0.072f;

    hoveredItem_ = -1;
    for (int i = 0; i < 4; ++i) {
        const Rectangle rect = gui::draw::centeredRect(cx, startY + i * gap, buttonW, buttonH);
        if (gui::page::isHovered(rect, mouse)) {
            hoveredItem_ = i;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
        return false;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return false;
    }

    switch (hoveredItem_) {
        case 0:
            setup_.loadFilePath.clear();
            screen_ = AppScreen::NEW_GAME_NUM_PLAYERS;
            hoveredItem_ = -1;
            return false;
        case 1:
            screen_ = AppScreen::LOAD_GAME;
            hoveredItem_ = -1;
            return false;
        case 3:
            CloseWindow();
            return false;
        default:
            return false;
    }
#else
    return false;
#endif
}
