#include "ui/GUIView.hpp"

#if NIMONSPOLY_ENABLE_RAYLIB
#include <array>

#include "../components/GUIViewDraw.hpp"
#include "../GuiMenuLayout.hpp"
#include "PageShared.hpp"
#endif

void GUIView::drawNumPlayers() {
#if NIMONSPOLY_ENABLE_RAYLIB
    AssetManager& am = AssetManager::get();
    ClearBackground(RL_BLACK);
    gui::page::drawMenuBackground(am);

    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const gui::menu::NumPlayersLayout layout = gui::menu::makeNumPlayersLayout(W, H);

    gui::draw::drawPanel(layout.panel, gui::menu::makeColor(6, 12, 24, 190));
    gui::draw::drawCenteredText(am, "title", "Choose Player Count", H * 0.042f, RL_WHITE, layout.titleY);
    gui::draw::drawMenuButton(am, "< Back", layout.backButton, hoveredItem_ == 20, false);

    const std::array<const char*, 3> labels{"2 Players", "3 Players", "4 Players"};
    for (int i = 0; i < 3; ++i) {
        const bool selected = (setup_.numPlayers == i + 2);
        gui::draw::drawMenuButton(am,
                                  labels[static_cast<size_t>(i)],
                                  layout.optionButtons[static_cast<size_t>(i)],
                                  hoveredItem_ == i,
                                  selected);
    }

    gui::draw::drawMenuButton(am, "Next >", layout.nextButton, hoveredItem_ == 10, false);
#endif
}

bool GUIView::handlePlayerCountInput() {
#if NIMONSPOLY_ENABLE_RAYLIB
    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const gui::menu::NumPlayersLayout layout = gui::menu::makeNumPlayersLayout(W, H);
    const Vector2 mouse = GetMousePosition();

    hoveredItem_ = -1;
    if (gui::page::isHovered(layout.backButton, mouse)) {
        hoveredItem_ = 20;
    }
    for (int i = 0; i < 3; ++i) {
        if (gui::page::isHovered(layout.optionButtons[static_cast<size_t>(i)], mouse)) {
            hoveredItem_ = i;
        }
    }
    if (gui::page::isHovered(layout.nextButton, mouse)) {
        hoveredItem_ = 10;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        screen_ = AppScreen::LANDING;
        hoveredItem_ = -1;
        return false;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return false;
    }

    if (gui::page::isHovered(layout.backButton, mouse)) {
        screen_ = AppScreen::LANDING;
        hoveredItem_ = -1;
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        if (gui::page::isHovered(layout.optionButtons[static_cast<size_t>(i)], mouse)) {
            setup_.numPlayers = i + 2;
        }
    }

    if (gui::page::isHovered(layout.nextButton, mouse)) {
        gui::menu::prepareSetupPlayers(setup_);
        custPlayerTab_ = 0;
        screen_ = AppScreen::NEW_GAME_CUST_PLAYER;
        hoveredItem_ = -1;
    }
#endif
    return false;
}
