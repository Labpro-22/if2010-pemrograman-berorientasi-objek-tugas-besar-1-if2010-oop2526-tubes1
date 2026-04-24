#include "ui/GUIView.hpp"

#if NIMONSPOLY_ENABLE_RAYLIB
#include <string>

#include "../components/GUIViewDraw.hpp"
#include "PageShared.hpp"
#endif

void GUIView::drawGameOver() {
#if NIMONSPOLY_ENABLE_RAYLIB
    AssetManager& am = AssetManager::get();
    ClearBackground(gui::draw::tileColor(::Color::DARK_BLUE));
    gui::page::drawMenuBackground(am);

    const float H = static_cast<float>(GetScreenHeight());
    const float cy = H * 0.5f;

    gui::draw::drawCenteredText(am, "title", "GAME OVER", H * 0.08f, gui::menu::makeColor(235, 240, 248), cy - H * 0.22f);

    float y = cy - H * 0.08f;
    if (!winnerInfo_.winners.empty()) {
        gui::draw::drawCenteredText(am,
                                    "bold",
                                    "Winner: " + winnerInfo_.winners.front(),
                                    H * 0.045f,
                                    gui::menu::makeColor(0xff, 0xf2, 0x00),
                                    y);
        y += H * 0.07f;
    }

    for (size_t i = 0; i < winnerInfo_.players.size(); ++i) {
        const auto& player = winnerInfo_.players[i];
        const std::string line =
            std::to_string(i + 1) + ". " + player.username + "  -  " + player.money.toString();
        gui::draw::drawCenteredText(am,
                                    "regular",
                                    line,
                                    H * 0.028f,
                                    i == 0 ? gui::menu::makeColor(0xff, 0xf2, 0x00)
                                           : gui::menu::makeColor(210, 220, 235),
                                    y + static_cast<float>(i) * H * 0.045f);
    }

    gui::draw::drawCenteredText(am,
                                "regular",
                                "Press ESC or click to return to menu",
                                H * 0.022f,
                                gui::menu::makeColor(150, 160, 180),
                                H * 0.92f);
#endif
}
