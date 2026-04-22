#include "ui/GUIView.hpp"

#include "core/state/header/GameStateView.hpp"
#include "ui/AssetManager.hpp"

#if NIMONSPOLY_ENABLE_SFML
#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <string>
#endif

GUIView::GUIView(sf::RenderWindow& window) : window(&window) {}

#if NIMONSPOLY_ENABLE_SFML

// Exact hex colors from Figma design
static sf::Color tileColor(Color c) {
    switch (c) {
        case Color::BROWN:      return sf::Color(0xda, 0xcc, 0xc1);
        case Color::LIGHT_BLUE: return sf::Color(0x2e, 0xc7, 0xff);
        case Color::PINK:       return sf::Color(0xee, 0x2a, 0x89);
        case Color::ORANGE:     return sf::Color(0xff, 0x77, 0x00);
        case Color::RED:        return sf::Color(0xbe, 0x17, 0x1a);
        case Color::YELLOW:     return sf::Color(0xff, 0xf2, 0x00);
        case Color::GREEN:      return sf::Color(0x00, 0x93, 0x64);
        case Color::DARK_BLUE:  return sf::Color(0x12, 0x48, 0x72);
        case Color::GRAY:       return sf::Color(0xa0, 0xa0, 0xa0);
        default:                return sf::Color(0x37, 0x3c, 0x46);
    }
}

static sf::Color tokenColor(int playerIndex) {
    constexpr std::array<sf::Color, 4> colors{
        sf::Color(0x00, 0xc8, 0xff),   // cyan
        sf::Color(0xff, 0x2d, 0x8a),   // pink
        sf::Color(0xff, 0xf2, 0x00),   // yellow
        sf::Color(0x00, 0xff, 0xb0),   // green
    };
    return colors[static_cast<size_t>(playerIndex) % 4];
}

// Draw one tile card at screen pos with optional rotation.
// rotDeg: 0=top/normal, 90=left col, 180=bottom row, 270=right col
static void drawTileCard(sf::RenderWindow& rw,
                          sf::Vector2f pos, float tileSz,
                          const TileView& tv,
                          AssetManager& am) {
    // Rotation: content always faces outward from board edge
    const int side = tv.index / 10;
    float rotDeg = 0.f;
    if      (side == 0) rotDeg = 180.f;
    else if (side == 1) rotDeg =  90.f;
    else if (side == 3) rotDeg = 270.f;

    sf::Vector2f center{pos.x + tileSz * 0.5f, pos.y + tileSz * 0.5f};
    sf::Transform tfm;
    tfm.rotate(sf::degrees(rotDeg), center);
    sf::RenderStates rs(tfm);

    const sf::Color grpColor = tileColor(tv.color);
    const bool hasGroup = (tv.color != Color::DEFAULT);

    // ── 1. Card background ──────────────────────────────────────────────────
    sf::RectangleShape bg({tileSz, tileSz});
    bg.setPosition(pos);
    bg.setFillColor(hasGroup ? sf::Color(250, 250, 248) : sf::Color(50, 54, 66));
    bg.setOutlineThickness(1.f);
    bg.setOutlineColor(sf::Color(18, 20, 25));
    rw.draw(bg, rs);

    // ── 2. Photo texture (top 62% of tile) ──────────────────────────────────
    const float photoFrac = 0.62f;
    const float photoH    = tileSz * photoFrac;
    const sf::Texture* tex = am.tileTexture(tv.code);

    if (tex) {
        auto texSz = tex->getSize();
        float scaleX = tileSz / static_cast<float>(texSz.x);
        float scaleY = photoH / static_cast<float>(texSz.y);
        float scale  = std::min(scaleX, scaleY);
        float sw = static_cast<float>(texSz.x) * scale;
        float sh = static_cast<float>(texSz.y) * scale;
        sf::Sprite sprite(*tex);
        sprite.setScale({scale, scale});
        sprite.setPosition({pos.x + (tileSz - sw) * 0.5f,
                            pos.y + (photoH - sh) * 0.5f});
        rw.draw(sprite, rs);
    } else if (hasGroup) {
        // Fallback gradient: semi-transparent group color
        sf::RectangleShape fill({tileSz, photoH});
        fill.setPosition(pos);
        fill.setFillColor(sf::Color(grpColor.r, grpColor.g, grpColor.b, 100));
        rw.draw(fill, rs);
    }

    // ── 3. Color accent strip (between photo and badge) ─────────────────────
    if (hasGroup) {
        sf::RectangleShape strip({tileSz, tileSz * 0.08f});
        strip.setPosition({pos.x, pos.y + photoH});
        strip.setFillColor(grpColor);
        rw.draw(strip, rs);
    }

    // ── 4. Price badge (bottom 22% of tile) ─────────────────────────────────
    const float badgeFrac = 0.22f;
    const float badgeY    = pos.y + tileSz * (1.f - badgeFrac);
    sf::RectangleShape badge({tileSz, tileSz * badgeFrac});
    badge.setPosition({pos.x, badgeY});
    badge.setFillColor(sf::Color(255, 255, 255, 210));
    rw.draw(badge, rs);

    // ── 5. Tile code label ───────────────────────────────────────────────────
    const sf::Font& font = am.font("bold");
    unsigned charSz = static_cast<unsigned>(tileSz * 0.17f);
    if (charSz < 6u) charSz = 6u;
    sf::Text label(font, tv.code, charSz);
    sf::Color textColor = hasGroup ? grpColor : sf::Color(200, 200, 200);
    // Yellow is hard to read on white badge — darken it
    if (tv.color == Color::YELLOW)
        textColor = sf::Color(0x90, 0x87, 0x00);
    label.setFillColor(textColor);
    auto lb = label.getLocalBounds();
    label.setOrigin({lb.position.x + lb.size.x * 0.5f,
                     lb.position.y + lb.size.y * 0.5f});
    label.setPosition({pos.x + tileSz * 0.5f,
                       badgeY + tileSz * badgeFrac * 0.5f});
    rw.draw(label, rs);
}

#endif  // NIMONSPOLY_ENABLE_SFML

void GUIView::showBoard(const GameStateView& state) {
#if NIMONSPOLY_ENABLE_SFML
    if (!window) return;

    sf::RenderWindow& rw = *window;
    AssetManager& am = AssetManager::get();

    const sf::Vector2u winSize = rw.getSize();
    const float minDim  = std::min<float>(static_cast<float>(winSize.x),
                                           static_cast<float>(winSize.y));
    const float boardSz = minDim * 0.86f;
    const sf::Vector2f origin{
        (static_cast<float>(winSize.x) - boardSz) * 0.5f,
        (static_cast<float>(winSize.y) - boardSz) * 0.5f,
    };

    constexpr int EDGE  = 10;
    constexpr int SIDE  = EDGE + 1;   // 11
    constexpr int TOTAL = EDGE * 4;   // 40
    const float tileSz  = boardSz / static_cast<float>(SIDE);
    const float innerSz = boardSz - 2.0f * tileSz;

    auto tilePos = [&](int idx) -> sf::Vector2f {
        const float edge = tileSz * static_cast<float>(SIDE);
        const int   sd   = idx / EDGE;
        const int   off  = idx % EDGE;
        switch (sd) {
            case 0:  return {origin.x + edge - tileSz - off * tileSz, origin.y + edge - tileSz};
            case 1:  return {origin.x,                                 origin.y + edge - tileSz - off * tileSz};
            case 2:  return {origin.x + off * tileSz,                  origin.y};
            default: return {origin.x + edge - tileSz,                 origin.y + off * tileSz};
        }
    };

    // ── Clear + inner board area ─────────────────────────────────────────────
    rw.clear(sf::Color(0x1a, 0x27, 0x44));   // dark navy from Figma

    sf::RectangleShape inner({innerSz, innerSz});
    inner.setPosition({origin.x + tileSz, origin.y + tileSz});
    inner.setFillColor(sf::Color(0x0d, 0x18, 0x2e));
    rw.draw(inner);

    // Center logo — "NIMONS POLY" text
    {
        const sf::Font& titleFont = am.font("title");
        float logoSz = innerSz * 0.28f;
        unsigned charSz = static_cast<unsigned>(logoSz);
        sf::Text line1(titleFont, std::string("NIMONS"), charSz);
        sf::Text line2(titleFont, std::string("POLY"),   charSz);
        line1.setFillColor(sf::Color(255, 255, 255));
        line2.setFillColor(sf::Color(255, 255, 255));
        float cx = origin.x + tileSz + innerSz * 0.5f;
        float cy = origin.y + tileSz + innerSz * 0.5f;
        auto b1 = line1.getLocalBounds();
        auto b2 = line2.getLocalBounds();
        line1.setOrigin({b1.position.x + b1.size.x * 0.5f, b1.position.y + b1.size.y});
        line2.setOrigin({b2.position.x + b2.size.x * 0.5f, b2.position.y});
        line1.setPosition({cx, cy - logoSz * 0.08f});
        line2.setPosition({cx, cy + logoSz * 0.08f});
        rw.draw(line1);
        rw.draw(line2);
    }

    // ── Tiles ────────────────────────────────────────────────────────────────
    const int numTiles = std::min<int>(TOTAL, static_cast<int>(state.tiles.size()));
    for (int i = 0; i < numTiles; ++i) {
        drawTileCard(rw, tilePos(i), tileSz,
                     state.tiles[static_cast<size_t>(i)], am);
    }

    // ── Player tokens ────────────────────────────────────────────────────────
    const float tokenR       = tileSz * 0.16f;
    const float tokenSpacing = tokenR * 2.4f;

    std::array<std::vector<int>, TOTAL> playersAt{};
    for (int p = 0; p < static_cast<int>(state.players.size()); ++p) {
        const auto& pv = state.players[static_cast<size_t>(p)];
        if (pv.status != PlayerStatus::BANKRUPT) {
            int pos = pv.position;
            if (pos >= 0 && pos < TOTAL)
                playersAt[static_cast<size_t>(pos)].push_back(p);
        }
    }

    for (int i = 0; i < TOTAL; ++i) {
        const auto& ps = playersAt[static_cast<size_t>(i)];
        if (ps.empty()) continue;
        const sf::Vector2f base = tilePos(i);
        const float cx = base.x + tileSz * 0.5f;
        const float cy = base.y + tileSz * 0.5f;   // center of tile
        const float totalW = static_cast<float>(ps.size()) * tokenSpacing;
        float startX = cx - totalW * 0.5f + tokenSpacing * 0.5f;

        for (int playerIdx : ps) {
            sf::CircleShape token(tokenR);
            token.setFillColor(tokenColor(playerIdx));
            token.setOutlineThickness(2.f);
            token.setOutlineColor(sf::Color(255, 255, 255, 200));
            token.setOrigin({tokenR, tokenR});
            token.setPosition({startX, cy});
            rw.draw(token);
            startX += tokenSpacing;
        }
    }

    rw.display();
#else
    (void)state;
#endif
}

void GUIView::showDiceResult(int d1, int d2, const string& playerName) {
    (void)d1;
    (void)d2;
    (void)playerName;
}

void GUIView::showPlayerLanding(const string& playerName, const string& tileName) {
    (void)playerName;
    (void)tileName;
}

void GUIView::showPropertyCard(const PropertyInfo& propertyInfo) {
    (void)propertyInfo;
}

void GUIView::showPlayerProperties(const vector<PropertyInfo>& list) {
    (void)list;
}

void GUIView::showBuyPrompt(const PropertyInfo& propertyInfo, Money playerMoney) {
    (void)propertyInfo;
    (void)playerMoney;
}

void GUIView::showRentPayment(const RentInfo& rentInfo) {
    (void)rentInfo;
}

void GUIView::showTaxPrompt(const TaxInfo& taxInfo) {
    (void)taxInfo;
}

void GUIView::showAuctionState(const AuctionState& auctionState) {
    (void)auctionState;
}

void GUIView::showFestivalPrompt(const vector<PropertyInfo>& ownedProperties) {
    (void)ownedProperties;
}

void GUIView::showBankruptcy(const BankruptcyInfo& bankruptcyInfo) {
    (void)bankruptcyInfo;
}

void GUIView::showLiquidationPanel(const LiquidationState& liquidationState) {
    (void)liquidationState;
}

void GUIView::showCardDrawn(const CardInfo& cardInfo) {
    (void)cardInfo;
}

void GUIView::showSkillCardHand(const vector<CardInfo>& cards) {
    (void)cards;
}

void GUIView::showTransactionLog(const vector<LogEntry>& entries) {
    (void)entries;
}

void GUIView::showWinner(const WinnerInfo& winInfo) {
    (void)winInfo;
}

void GUIView::showJailStatus(const JailInfo& jailInfo) {
    (void)jailInfo;
}

void GUIView::showMessage(const string& message) {
    (void)message;
}

void GUIView::showBuildMenu(const BuildMenuState& buildMenuState) {
    (void)buildMenuState;
}

void GUIView::showMortgageMenu(const MortgageMenuState& mortgageMenuState) {
    (void)mortgageMenuState;
}

void GUIView::showRedeemMenu(const RedeemMenuState& redeemMenuState) {
    (void)redeemMenuState;
}

void GUIView::showDropCardPrompt(const vector<CardInfo>& cards) {
    (void)cards;
}

void GUIView::showSaveLoadStatus(const string& message) {
    (void)message;
}

void GUIView::showTurnInfo(const string& playerName, int turnNum, int maxTurn) {
    (void)playerName;
    (void)turnNum;
    (void)maxTurn;
}

void GUIView::showMainMenu() {}

void GUIView::showPlayerOrder(const vector<string>& orderedNames) {
    (void)orderedNames;
}

void GUIView::showDoubleBonusTurn(const string& playerName, int doubleCount) {
    (void)playerName;
    (void)doubleCount;
}

void GUIView::showAuctionWinner(const AuctionSummary& summary) {
    (void)summary;
}

void GUIView::showFestivalReinforced(const FestivalEffectInfo& info) {
    (void)info;
}

void GUIView::showFestivalAtMax(const FestivalEffectInfo& info) {
    (void)info;
}

void GUIView::showJailEntry(const JailEntryInfo& info) {
    (void)info;
}

void GUIView::showCardEffect(const CardEffectInfo& info) {
    (void)info;
}

void GUIView::showLiquidationResult(bool canCover, Money finalBalance) {
    (void)canCover;
    (void)finalBalance;
}
