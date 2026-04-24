#pragma once

#if NIMONSPOLY_ENABLE_RAYLIB
#include <cstddef>
#include <string>

#include "../components/GUIViewDraw.hpp"
#include "../GuiMenuLayout.hpp"
#include "ui/AssetManager.hpp"
#include "ui/RaylibCompat.hpp"

namespace gui::page {
    inline void drawMenuBackground(AssetManager& am) {
        const Texture2D* globeTex = am.texture("assets/bg/Globe.png");
        if (globeTex) {
            gui::draw::drawSpriteCoverScreen(globeTex);
        } else {
            gui::draw::drawGlobeBackground(GetScreenWidth(), GetScreenHeight());
        }
    }

    inline bool isHovered(const Rectangle& rect, Vector2 point) {
        return CheckCollisionPointRec(point, rect);
    }

    inline void appendTextInput(std::string& target, size_t maxLen) {
        for (int codepoint = GetCharPressed(); codepoint > 0; codepoint = GetCharPressed()) {
            if (codepoint >= 32 && codepoint < 127 && target.size() < maxLen) {
                target += static_cast<char>(codepoint);
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !target.empty()) {
            target.pop_back();
        }
    }
}
#endif
