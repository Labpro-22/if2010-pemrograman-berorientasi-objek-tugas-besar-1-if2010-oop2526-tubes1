#include "ui/GUIView.hpp"

#if NIMONSPOLY_ENABLE_RAYLIB
#include <string>

#include "../components/GUIViewDraw.hpp"
#include "../GuiMenuLayout.hpp"
#include "PageShared.hpp"
#endif

void GUIView::drawLoadGame() {
#if NIMONSPOLY_ENABLE_RAYLIB
    AssetManager& am = AssetManager::get();
    ClearBackground(RL_BLACK);
    gui::page::drawMenuBackground(am);

    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const float cx = W * 0.5f;
    const float cy = H * 0.5f;

    const Rectangle backRect = gui::draw::centeredRect(cx, cy - H * 0.30f, 140.f, 52.f);
    const Rectangle inputRect = Rectangle{cx - H * 0.27f, cy - H * 0.01f, H * 0.54f, 54.f};
    const Rectangle loadRect = gui::draw::centeredRect(cx, cy + H * 0.14f, 170.f, 56.f);

    gui::draw::drawMenuButton(am, "< Back", backRect, hoveredItem_ == 20, false);
    gui::draw::drawCenteredText(am, "title", "Load Game", H * 0.042f, RL_WHITE, cy - H * 0.20f);
    gui::draw::drawCenteredText(am, "bold", "Enter save file path:", H * 0.03f, RL_WHITE, cy - H * 0.10f);

    gui::draw::drawPanel(inputRect, gui::menu::makeColor(16, 22, 34, 220),
                         gui::menu::makeColor(255, 255, 255, 70));
    const std::string display = setup_.loadFilePath.empty() ? "saves/game.json" : setup_.loadFilePath;
    const float fontSize = H * 0.026f;
    const Font& font = am.font("title");
    const Vector2 textSize = MeasureTextEx(font, display.c_str(), fontSize, 0.f);
    DrawTextEx(font,
               display.c_str(),
               Vector2{
                   inputRect.x + inputRect.width * 0.04f,
                   inputRect.y + inputRect.height * 0.5f - textSize.y * 0.5f,
               },
               fontSize,
               0.f,
               setup_.loadFilePath.empty() ? gui::menu::makeColor(100, 120, 150)
                                           : gui::menu::makeColor(230, 240, 255));

    gui::draw::drawMenuButton(am, "Load", loadRect, hoveredItem_ == 0, false);
#endif
}

bool GUIView::handleLoadGameInput() {
#if NIMONSPOLY_ENABLE_RAYLIB
    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const float cx = W * 0.5f;
    const float cy = H * 0.5f;
    const Vector2 mouse = GetMousePosition();

    const Rectangle backRect = gui::draw::centeredRect(cx, cy - H * 0.30f, 140.f, 52.f);
    const Rectangle loadRect = gui::draw::centeredRect(cx, cy + H * 0.14f, 170.f, 56.f);

    gui::page::appendTextInput(setup_.loadFilePath, 128);

    hoveredItem_ = -1;
    if (gui::page::isHovered(backRect, mouse)) {
        hoveredItem_ = 20;
    } else if (gui::page::isHovered(loadRect, mouse)) {
        hoveredItem_ = 0;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        screen_ = AppScreen::LANDING;
        hoveredItem_ = -1;
        return false;
    }

    if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && !setup_.loadFilePath.empty()) {
        screen_ = AppScreen::IN_GAME;
        return true;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return false;
    }

    if (gui::page::isHovered(backRect, mouse)) {
        screen_ = AppScreen::LANDING;
        hoveredItem_ = -1;
        return false;
    }

    if (gui::page::isHovered(loadRect, mouse) && !setup_.loadFilePath.empty()) {
        screen_ = AppScreen::IN_GAME;
        return true;
    }
#endif
    return false;
}
