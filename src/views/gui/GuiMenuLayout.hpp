#pragma once

#if NIMONSPOLY_ENABLE_RAYLIB
#include <algorithm>
#include <array>
#include <string>
#include <utility>
#include <vector>

#include "ui/AppScreen.hpp"
#include "ui/RaylibCompat.hpp"

namespace gui::menu {
    constexpr int kSetupColorCount = 4;
    constexpr int kCharacterCount = 4;

    inline RaylibColor makeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
        return RaylibColor{r, g, b, a};
    }

    inline const std::array<RaylibColor, kSetupColorCount>& setupPalette() {
        static const std::array<RaylibColor, kSetupColorCount> colors{
            makeColor(0xe3, 0x4b, 0x4b),
            makeColor(0xf4, 0xd3, 0x1c),
            makeColor(0x41, 0x8d, 0xff),
            makeColor(0x32, 0xc4, 0x71),
        };
        return colors;
    }

    inline const std::array<const char*, kSetupColorCount>& setupColorKeys() {
        static const std::array<const char*, kSetupColorCount> keys{
            "red", "yellow", "blue", "green",
        };
        return keys;
    }

    inline const std::array<const char*, kCharacterCount>& characterKeys() {
        static const std::array<const char*, kCharacterCount> keys{
            "cruiser", "rocket", "shuttle", "ufo",
        };
        return keys;
    }

    inline const std::array<const char*, kCharacterCount>& characterLabels() {
        static const std::array<const char*, kCharacterCount> labels{
            "Cruiser", "Rocket", "Shuttle", "UFO",
        };
        return labels;
    }

    inline int normalizeSetupIndex(int value, int modulo) {
        if (modulo <= 0) {
            return 0;
        }
        return (value % modulo + modulo) % modulo;
    }

    inline int selectedPlayerColor(const SetupState& setup, int playerIndex) {
        if (playerIndex >= 0 && playerIndex < static_cast<int>(setup.playerColors.size())) {
            return normalizeSetupIndex(setup.playerColors[static_cast<size_t>(playerIndex)], kSetupColorCount);
        }
        return normalizeSetupIndex(playerIndex, kSetupColorCount);
    }

    inline int selectedPlayerCharacter(const SetupState& setup, int playerIndex) {
        if (playerIndex >= 0 && playerIndex < static_cast<int>(setup.playerCharacters.size())) {
            return normalizeSetupIndex(setup.playerCharacters[static_cast<size_t>(playerIndex)], kCharacterCount);
        }
        return normalizeSetupIndex(playerIndex, kCharacterCount);
    }

    inline bool colorTakenByOtherPlayer(const SetupState& setup, int currentPlayerIndex, int colorIndex) {
        for (int i = 0; i < static_cast<int>(setup.playerColors.size()); ++i) {
            if (i == currentPlayerIndex) {
                continue;
            }
            if (selectedPlayerColor(setup, i) == colorIndex) {
                return true;
            }
        }
        return false;
    }

    inline std::string colorPreviewPath(const SetupState& setup, int playerIndex, int colorIndex) {
        const int characterIndex = selectedPlayerCharacter(setup, playerIndex);
        return "assets/components/characters/" +
               std::string(characterKeys()[static_cast<size_t>(characterIndex)]) +
               "_" + setupColorKeys()[static_cast<size_t>(normalizeSetupIndex(colorIndex, kSetupColorCount))] + ".png";
    }

    inline std::string grayPreviewPath(const SetupState& setup, int playerIndex) {
        const int characterIndex = selectedPlayerCharacter(setup, playerIndex);
        return "assets/components/characters/" +
               std::string(characterKeys()[static_cast<size_t>(characterIndex)]) +
               "_gray.png";
    }

    inline void prepareSetupPlayers(SetupState& setup) {
        const size_t count = static_cast<size_t>(std::max(0, setup.numPlayers));
        setup.playerNames.resize(count);
        setup.isComputer.resize(count, false);

        std::vector<int> colors(count, -1);
        std::array<bool, kSetupColorCount> usedColors{};
        for (size_t i = 0; i < count && i < setup.playerColors.size(); ++i) {
            const int color = normalizeSetupIndex(setup.playerColors[i], kSetupColorCount);
            if (!usedColors[static_cast<size_t>(color)]) {
                colors[i] = color;
                usedColors[static_cast<size_t>(color)] = true;
            }
        }

        for (size_t i = 0; i < count; ++i) {
            if (colors[i] != -1) {
                continue;
            }
            for (int candidate = 0; candidate < kSetupColorCount; ++candidate) {
                if (!usedColors[static_cast<size_t>(candidate)]) {
                    colors[i] = candidate;
                    usedColors[static_cast<size_t>(candidate)] = true;
                    break;
                }
            }
            if (colors[i] == -1) {
                colors[i] = normalizeSetupIndex(static_cast<int>(i), kSetupColorCount);
            }
        }
        setup.playerColors = std::move(colors);

        std::vector<int> characters(count, 0);
        for (size_t i = 0; i < count; ++i) {
            if (i < setup.playerCharacters.size()) {
                characters[i] = normalizeSetupIndex(setup.playerCharacters[i], kCharacterCount);
            } else {
                characters[i] = normalizeSetupIndex(static_cast<int>(i), kCharacterCount);
            }
        }
        setup.playerCharacters = std::move(characters);
    }

    inline Vector2 rectCenter(const Rectangle& rect) {
        return Vector2{rect.x + rect.width * 0.5f, rect.y + rect.height * 0.5f};
    }

    inline Rectangle centeredRect(float centerX, float centerY, float width, float height) {
        return Rectangle{centerX - width * 0.5f, centerY - height * 0.5f, width, height};
    }

    struct NumPlayersLayout {
        Rectangle panel{};
        Rectangle backButton{};
        std::array<Rectangle, 3> optionButtons{};
        Rectangle nextButton{};
        float titleY{0.f};
    };

    inline NumPlayersLayout makeNumPlayersLayout(float W, float H) {
        NumPlayersLayout layout{};
        const float panelW = std::min(W * 0.40f, 560.f);
        const float panelH = std::min(H * 0.50f, 480.f);
        layout.panel = Rectangle{
            W * 0.5f - panelW * 0.5f,
            H * 0.5f - panelH * 0.5f,
            panelW,
            panelH,
        };

        const float buttonW = panelW * 0.66f;
        const float buttonH = std::min(H * 0.065f, 70.f);
        const float gap = std::min(H * 0.028f, 28.f);
        const float optionStartY = layout.panel.y + panelH * 0.38f;
        const float buttonX = layout.panel.x + panelW * 0.5f;

        layout.backButton = centeredRect(layout.panel.x + 66.f,
                                         layout.panel.y - H * 0.055f,
                                         128.f,
                                         std::min(H * 0.05f, 52.f));
        layout.titleY = layout.panel.y + panelH * 0.18f;
        for (int i = 0; i < 3; ++i) {
            layout.optionButtons[static_cast<size_t>(i)] =
                centeredRect(buttonX, optionStartY + i * (buttonH + gap), buttonW, buttonH);
        }
        layout.nextButton = centeredRect(buttonX,
                                         layout.panel.y + panelH - H * 0.075f,
                                         166.f,
                                         std::min(H * 0.055f, 56.f));
        return layout;
    }

    struct CustomizePlayerLayout {
        Rectangle sidePanel{};
        Rectangle mainPanel{};
        Rectangle backButton{};
        Rectangle nextButton{};
        Rectangle nameBox{};
        std::array<Rectangle, kSetupColorCount> colorSlots{};
        Rectangle characterLeft{};
        Rectangle characterRight{};
        Rectangle characterPreview{};
        Rectangle humanButton{};
        Rectangle computerButton{};
        Vector2 tabSize{};
        float tabStartY{0.f};
        float tabGap{0.f};
        float nameLabelY{0.f};
        float colorLabelY{0.f};
        float characterLabelY{0.f};
        float typeLabelY{0.f};
    };

    inline CustomizePlayerLayout makeCustomizePlayerLayout(float W, float H, int numPlayers) {
        CustomizePlayerLayout layout{};
        const float mainPanelW = std::min(W * 0.48f, 720.f);
        const float mainPanelH = std::min(H * 0.64f, 690.f);
        const float sidePanelW = std::min(W * 0.12f, 150.f);
        const float gapX = std::min(W * 0.03f, 42.f);
        const float totalW = sidePanelW + gapX + mainPanelW;
        const float left = W * 0.5f - totalW * 0.5f;
        const float top = H * 0.19f;

        layout.sidePanel = Rectangle{left, top, sidePanelW, mainPanelH};
        layout.mainPanel = Rectangle{left + sidePanelW + gapX, top, mainPanelW, mainPanelH};
        layout.backButton = centeredRect(layout.sidePanel.x + layout.sidePanel.width * 0.5f,
                                         H * 0.10f,
                                         128.f,
                                         std::min(H * 0.05f, 52.f));
        layout.nextButton = centeredRect(layout.mainPanel.x + mainPanelW - 78.f,
                                         top + mainPanelH - 34.f,
                                         156.f,
                                         std::min(H * 0.055f, 56.f));

        const float railPad = std::min(H * 0.018f, 18.f);
        layout.tabGap = std::min(H * 0.012f, 14.f);
        const int safePlayers = std::max(1, numPlayers);
        const float railInnerH = mainPanelH - railPad * 2.f;
        const float rawTabH =
            (railInnerH - layout.tabGap * static_cast<float>(safePlayers - 1)) /
            static_cast<float>(safePlayers);
        layout.tabSize = Vector2{
            sidePanelW - railPad * 2.f,
            std::clamp(rawTabH, 58.f, 86.f),
        };
        const float totalTabsH =
            layout.tabSize.y * static_cast<float>(safePlayers) +
            layout.tabGap * static_cast<float>(safePlayers - 1);
        layout.tabStartY = top + (mainPanelH - totalTabsH) * 0.5f;

        const float padX = std::min(mainPanelW * 0.08f, 72.f);
        const float contentLeft = layout.mainPanel.x + padX;
        const float contentW = mainPanelW - padX * 2.f;
        const float nameBoxH = std::min(H * 0.06f, 58.f);
        const float colorGap = std::min(W * 0.012f, 18.f);
        const float colorSlotW = (contentW - colorGap * 3.f) / 4.f;
        const float colorSlotH = std::min(H * 0.12f, 118.f);
        const float characterPreviewW = std::min(contentW * 0.36f, 250.f);
        const float characterPreviewH = std::min(H * 0.13f, 132.f);
        const float arrowW = std::min(W * 0.045f, 66.f);
        const float arrowH = characterPreviewH * 0.78f;
        const float typeGap = std::min(W * 0.02f, 22.f);
        const float typeButtonW = (contentW - typeGap) * 0.5f;
        const float typeButtonH = std::min(H * 0.06f, 60.f);

        layout.nameLabelY = top + std::min(mainPanelH * 0.11f, 82.f);
        layout.nameBox = Rectangle{contentLeft, layout.nameLabelY + 26.f, contentW, nameBoxH};
        layout.colorLabelY = layout.nameBox.y + nameBoxH + 34.f;
        const float colorTop = layout.colorLabelY + 24.f;
        for (int i = 0; i < kSetupColorCount; ++i) {
            layout.colorSlots[static_cast<size_t>(i)] = Rectangle{
                contentLeft + i * (colorSlotW + colorGap),
                colorTop,
                colorSlotW,
                colorSlotH,
            };
        }

        layout.characterLabelY = colorTop + colorSlotH + 36.f;
        const float characterTop = layout.characterLabelY + 24.f;
        layout.characterPreview = centeredRect(layout.mainPanel.x + mainPanelW * 0.5f,
                                               characterTop + characterPreviewH * 0.5f,
                                               characterPreviewW,
                                               characterPreviewH);
        layout.characterLeft = centeredRect(layout.characterPreview.x - arrowW * 0.5f - 18.f,
                                            rectCenter(layout.characterPreview).y,
                                            arrowW,
                                            arrowH);
        layout.characterRight = centeredRect(layout.characterPreview.x +
                                                 layout.characterPreview.width +
                                                 arrowW * 0.5f + 18.f,
                                             rectCenter(layout.characterPreview).y,
                                             arrowW,
                                             arrowH);

        layout.typeLabelY = characterTop + characterPreviewH + 36.f;
        const float typeTop = layout.typeLabelY + 24.f;
        layout.humanButton = Rectangle{contentLeft, typeTop, typeButtonW, typeButtonH};
        layout.computerButton = Rectangle{contentLeft + typeButtonW + typeGap, typeTop, typeButtonW, typeButtonH};
        return layout;
    }

    inline Rectangle playerTabRect(const CustomizePlayerLayout& layout, int playerIndex) {
        return Rectangle{
            layout.sidePanel.x + (layout.sidePanel.width - layout.tabSize.x) * 0.5f,
            layout.tabStartY + playerIndex * (layout.tabSize.y + layout.tabGap),
            layout.tabSize.x,
            layout.tabSize.y,
        };
    }
}
#endif
