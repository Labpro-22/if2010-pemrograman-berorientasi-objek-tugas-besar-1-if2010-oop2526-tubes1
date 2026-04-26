#include "ui/GUIView.hpp"

#if NIMONSPOLY_ENABLE_RAYLIB
#include "ui/RaylibCompat.hpp"
#endif

void GUIView::renderCurrentScreen() {
    switch (screen_) {
        case AppScreen::LANDING:
            drawLandingPage();
            break;
        case AppScreen::NEW_GAME_NUM_PLAYERS:
            drawNumPlayers();
            break;
        case AppScreen::NEW_GAME_CUST_PLAYER:
            drawCustPlayer();
            break;
        case AppScreen::NEW_GAME_CUST_MAP:
            drawCustMap();
            break;
        case AppScreen::LOAD_GAME:
            drawLoadGame();
            break;
        case AppScreen::IN_GAME:
            break;
        case AppScreen::GAME_OVER:
            drawGameOver();
            break;
    }
}

bool GUIView::handleMenuInput() {
#if NIMONSPOLY_ENABLE_RAYLIB
    switch (screen_) {
        case AppScreen::LANDING:
            return handleLandingInput();
        case AppScreen::NEW_GAME_NUM_PLAYERS:
            return handlePlayerCountInput();
        case AppScreen::NEW_GAME_CUST_PLAYER:
            return handlePlayerCustomizeInput();
        case AppScreen::NEW_GAME_CUST_MAP:
            return handleMapCustomizeInput();
        case AppScreen::LOAD_GAME:
            return handleLoadGameInput();
        case AppScreen::GAME_OVER:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_ESCAPE)) {
                screen_ = AppScreen::LANDING;
                hoveredItem_ = -1;
            }
            return false;
        case AppScreen::IN_GAME:
            return false;
    }
#endif
    return false;
}
