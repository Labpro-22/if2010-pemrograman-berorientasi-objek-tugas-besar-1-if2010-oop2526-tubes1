#include "../../include/views/GameWindow.hpp"
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <raylib.h>
#include <sstream>

static const int SCREEN_W = 1280;
static const int SCREEN_H = 800;
static const int BOARD_X = 16;
static const int BOARD_Y = 16;
static const int BOARD_W = 820;
static const int BOARD_H = 680;
static const int CMD_X = 16;
static const int CMD_Y = 700;
static const int CMD_W = 820;
static const int CMD_H = 96;
static const int SIDE_X = 844;
static const int SIDE_Y = 16;
static const int SIDE_W = 424;
static const int SIDE_H = 780;
static const int CORNER_SIZE = 80;
static const int TILE_W_H = 56;
static const int TILE_H_V = 56;

static const Color C_BG = {255, 255, 255, 255};
static const Color C_PANEL = {248, 248, 248, 255};
static const Color C_PANEL_ALT = {237, 237, 237, 255};
static const Color C_BORDER = {76, 56, 30, 255};
static const Color C_TEXT = {30, 20, 10, 255};
static const Color C_TEXT_DIM = {115, 89, 56, 255};
static const Color C_ACCENT = {38, 153, 51, 255};
static const Color C_ACCENT_BG = {191, 230, 196, 255};
static const Color C_GOOD = {25, 128, 51, 255};
static const Color C_WARN = {204, 140, 12, 255};
static const Color C_DANGER = {194, 25, 25, 255};
static const Color C_BTN_BG = {64, 46, 20, 255};
static const Color C_BTN_TEXT = {242, 229, 191, 255};
static const Color C_BOARD_BG = {209, 235, 209, 255};

static Color colorGroupToColor(const std::string &group) {
  if (group == "COKLAT")
    return {140, 69, 18, 255};
  if (group == "BIRU_MUDA")
    return {112, 191, 237, 255};
  if (group == "MERAH_MUDA")
    return {235, 115, 178, 255};
  if (group == "ORANGE")
    return {237, 140, 30, 255};
  if (group == "MERAH")
    return {217, 38, 38, 255};
  if (group == "KUNING")
    return {237, 209, 20, 255};
  if (group == "HIJAU")
    return {46, 173, 71, 255};
  if (group == "BIRU_TUA")
    return {20, 46, 184, 255};
  if (group == "ABU_ABU")
    return {153, 153, 166, 255};
  return {128, 110, 84, 255};
}

static Color playerIndexToColor(int i) {
  switch (i) {
  case 0:
    return {217, 38, 38, 255};
  case 1:
    return {38, 89, 204, 255};
  case 2:
    return {38, 153, 64, 255};
  case 3:
    return {204, 140, 12, 255};
  default:
    return {100, 100, 100, 255};
  }
}

static std::string formatMoney(int amount) {
  std::string s = std::to_string(amount);
  int insertPos = (int)s.length() - 3;
  while (insertPos > 0) {
    s.insert(insertPos, ".");
    insertPos -= 3;
  }
  return "M" + s;
}

static std::string truncate(const std::string &s, int maxLen) {
  if ((int)s.length() <= maxLen)
    return s;
  return s.substr(0, maxLen - 2) + "..";
}

GameWindow::GameWindow(int width, int height, const std::string &title)
    : screenW(width), screenH(height), fontLoaded(false) {

  boardRect = {(float)BOARD_X, (float)BOARD_Y, (float)BOARD_W, (float)BOARD_H};
  sidebarRect = {(float)SIDE_X, (float)SIDE_Y, (float)SIDE_W, (float)SIDE_H};
  cmdBarRect = {(float)CMD_X, (float)CMD_Y, (float)CMD_W, (float)CMD_H};

  InitWindow(width, height, title.c_str());
  SetTargetFPS(60);
}

GameWindow::~GameWindow() {
  if (fontLoaded)
    UnloadFont(pixelFont);
  CloseWindow();
}

void GameWindow::init() { fontLoaded = false; }

void GameWindow::updateState(const GameState &newState) { state = newState; }

void GameWindow::showPopup(const PopupState &p) { popup = p; }

void GameWindow::closePopup() { popup.type = PopupType::NONE; }

bool GameWindow::isRunning() const { return !WindowShouldClose(); }

void GameWindow::onCommand(const std::string &name, std::function<void()> cb) {
  commandCallbacks[name] = cb;
}

void GameWindow::onPopupOption(std::function<void(int)> cb) {
  popupCallback = cb;
}

void GameWindow::tick() {
  BeginDrawing();
  ClearBackground(C_BG);

  drawBoard();
  drawSidebar();
  drawCommandBar();

  if (popup.type != PopupType::NONE) {
    drawPopup();
  }

  EndDrawing();
}

void GameWindow::drawPixelText(const std::string &text, int x, int y, int size,
                               Color color) {
  if (fontLoaded) {
    DrawTextEx(pixelFont, text.c_str(), {(float)x, (float)y}, (float)size, 1.0f,
               color);
  } else {
    DrawText(text.c_str(), x, y, size, color);
  }
}

bool GameWindow::isHovered(Rectangle rect) const {
  Vector2 mouse = GetMousePosition();
  return CheckCollisionPointRec(mouse, rect);
}

void GameWindow::drawButton(const std::string &label, Rectangle rect, Color bg,
                            Color fg, bool hovered) {

  DrawRectangleRec({rect.x + 3, rect.y + 3, rect.width, rect.height},
                   {C_BORDER.r, C_BORDER.g, C_BORDER.b, 80});

  Color drawBg = hovered ? Color{(unsigned char)std::min(255, bg.r + 20),
                                 (unsigned char)std::min(255, bg.g + 20),
                                 (unsigned char)std::min(255, bg.b + 20), bg.a}
                         : bg;

  DrawRectangleRec(rect, drawBg);
  DrawRectangleLinesEx(rect, 2, C_BORDER);

  int fontSize = 6;

  int textW = (int)label.length() * (fontSize);
  int textX = (int)rect.x + ((int)rect.width - textW) / 2;
  int textY = (int)rect.y + ((int)rect.height - fontSize) / 2 - 1;
  drawPixelText(label, textX, textY, fontSize, fg);
}

Color GameWindow::getTileColor(const std::string &colorGroup) const {
  return colorGroupToColor(colorGroup);
}

Color GameWindow::getPlayerColor(int playerIndex) const {
  return playerIndexToColor(playerIndex);
}

Rectangle GameWindow::getTileRect(int tileIndex) const {

  float bx = boardRect.x;
  float by = boardRect.y;
  float bw = boardRect.width;
  float bh = boardRect.height;

  float cs = CORNER_SIZE;
  float tw = (bw - 2 * cs) / 9.0f;
  float th = (bh - 2 * cs) / 9.0f;

  int i = ((tileIndex - 1) + 40) % 40;

  if (i == 0) {

    return {bx + bw - cs, by + bh - cs, cs, cs};
  }
  if (i >= 1 && i <= 9) {

    float x = bx + bw - cs - i * tw;
    return {x, by + bh - cs, tw, cs};
  }
  if (i == 10) {

    return {bx, by + bh - cs, cs, cs};
  }

  if (i >= 11 && i <= 19) {
    float y = by + bh - cs - (i - 10) * th;
    return {bx, y, cs, th};
  }
  if (i == 20) {

    return {bx, by, cs, cs};
  }

  if (i >= 21 && i <= 29) {
    float x = bx + cs + (i - 21) * tw;
    return {x, by, tw, cs};
  }
  if (i == 30) {

    return {bx + bw - cs, by, cs, cs};
  }

  if (i >= 31 && i <= 39) {
    float y = by + cs + (i - 31) * th;
    return {bx + bw - cs, y, cs, th};
  }

  return {bx, by, tw, th};
}

void GameWindow::drawBoard() {

  DrawRectangleRec(boardRect, C_BOARD_BG);
  DrawRectangleLinesEx(boardRect, 3, C_BORDER);

  float cx = boardRect.x + CORNER_SIZE;
  float cy = boardRect.y + CORNER_SIZE;
  float cw = boardRect.width - 2.0f * CORNER_SIZE;
  float ch = boardRect.height - 2.0f * CORNER_SIZE;
  DrawRectangle((int)cx, (int)cy, (int)cw, (int)ch, C_PANEL);
  DrawRectangleLinesEx({cx, cy, cw, ch}, 1,
                       {C_BORDER.r, C_BORDER.g, C_BORDER.b, 60});

  int turn = state.currentTurn;
  int maxTurn = state.maxTurn;
  std::string logoLine1 = "NIMONSPOLI";
  std::string turnInfo =
      "TURN " + std::to_string(turn) + " / " + std::to_string(maxTurn);

  int lx = (int)(cx + cw / 2);
  int ly = (int)(cy + ch / 2);
  drawPixelText(logoLine1, lx - 55, ly - 14, 10, C_ACCENT);
  drawPixelText(turnInfo, lx - 40, ly + 8, 6, C_TEXT_DIM);

  for (int idx = 1; idx <= 40; idx++) {

    const TileInfo *tilePtr = nullptr;
    for (const auto &t : state.tiles) {
      if (t.index == idx) {
        tilePtr = &t;
        break;
      }
    }
    if (!tilePtr)
      continue;
    drawTile(*tilePtr, getTileRect(idx));
  }

  drawPlayerPawns();
}

void GameWindow::drawTile(const TileInfo &tile, Rectangle rect) {

  DrawRectangleRec(rect, C_PANEL);
  DrawRectangleLinesEx(rect, 1, {C_BORDER.r, C_BORDER.g, C_BORDER.b, 120});

  bool isCorner = (rect.width >= (float)(CORNER_SIZE - 1) &&
                   rect.height >= (float)(CORNER_SIZE - 1));

  bool isSideBtm = (!isCorner && rect.height >= (float)(CORNER_SIZE - 1) &&
                    rect.y > boardRect.y + boardRect.height / 2.0f);

  bool isSideTop = (!isCorner && rect.height >= (float)(CORNER_SIZE - 1) &&
                    rect.y < boardRect.y + boardRect.height / 2.0f);

  bool isSideLft = (!isCorner && rect.width >= (float)(CORNER_SIZE - 1) &&
                    rect.x < boardRect.x + boardRect.width / 2.0f);

  if (!tile.colorGroup.empty()) {
    Color stripColor = getTileColor(tile.colorGroup);
    float stripSize = 12.0f;

    Rectangle stripRect;
    if (isSideBtm) {

      stripRect = {rect.x, rect.y, rect.width, stripSize};
    } else if (isSideTop) {

      stripRect = {rect.x, rect.y + rect.height - stripSize, rect.width,
                   stripSize};
    } else if (isSideLft) {

      stripRect = {rect.x + rect.width - stripSize, rect.y, stripSize,
                   rect.height};
    } else {

      stripRect = {rect.x, rect.y, stripSize, rect.height};
    }
    DrawRectangleRec(stripRect, stripColor);
  }

  if (tile.propStatus == "MORTGAGED") {
    DrawRectangleRec(rect, {C_WARN.r, C_WARN.g, C_WARN.b, 60});
  }

  int fontSize = isCorner ? 8 : 5;
  int tx = (int)rect.x + 3;
  int ty;

  if (isCorner) {
    ty = (int)rect.y + 30;
  } else if (isSideBtm) {

    ty = (int)rect.y + 14;
  } else if (isSideTop) {

    ty = (int)rect.y + 3;
  } else if (isSideLft) {

    ty = (int)rect.y + 4;
  } else {

    tx = (int)rect.x + 14;
    ty = (int)rect.y + 4;
  }

  drawPixelText(tile.kode, tx, ty, fontSize, C_TEXT);

  if (tile.price > 0 && !isCorner) {
    std::string priceStr = "M" + std::to_string(tile.price);
    int priceY = ty + fontSize + 3;
    drawPixelText(priceStr, tx, priceY, 4, C_GOOD);
  }

  if (tile.houseCount > 0 && !tile.hasHotel) {
    for (int h = 0; h < tile.houseCount; h++) {
      int hx, hy;
      if (isSideBtm) {

        hx = (int)rect.x + 2 + h * 9;
        hy = (int)rect.y + 2;
      } else if (isSideTop) {

        hx = (int)rect.x + 2 + h * 9;
        hy = (int)rect.y + (int)rect.height - 9;
      } else if (isSideLft) {

        hx = (int)rect.x + (int)rect.width - 9;
        hy = (int)rect.y + 2 + h * 9;
      } else {

        hx = (int)rect.x + 2;
        hy = (int)rect.y + 2 + h * 9;
      }
      DrawRectangle(hx, hy, 7, 7, C_GOOD);
    }
  }

  if (tile.hasHotel) {
    if (isSideBtm) {

      DrawRectangle((int)rect.x + 2, (int)rect.y + 2, (int)rect.width - 4, 7,
                    C_DANGER);
    } else if (isSideTop) {

      DrawRectangle((int)rect.x + 2, (int)rect.y + (int)rect.height - 9,
                    (int)rect.width - 4, 7, C_DANGER);
    } else if (isSideLft) {

      DrawRectangle((int)rect.x + (int)rect.width - 9, (int)rect.y + 2, 7,
                    (int)rect.height - 4, C_DANGER);
    } else {

      DrawRectangle((int)rect.x + 2, (int)rect.y + 2, 7, (int)rect.height - 4,
                    C_DANGER);
    }
  }

  if (tile.propStatus == "MORTGAGED") {
    DrawRectangle((int)rect.x + (int)rect.width - 14, (int)rect.y + 2, 12, 8,
                  C_WARN);
    drawPixelText("M", (int)rect.x + (int)rect.width - 12, (int)rect.y + 3, 4,
                  WHITE);
  }

  if (!tile.ownerName.empty() && tile.propStatus == "OWNED") {

    int ownerIdx = -1;
    for (int i = 0; i < (int)state.players.size(); i++) {
      if (state.players[i].username == tile.ownerName) {
        ownerIdx = i;
        break;
      }
    }
    if (ownerIdx >= 0) {
      Color oc = getPlayerColor(ownerIdx);
      DrawCircle((int)rect.x + (int)rect.width - 7, (int)rect.y + 7, 5, oc);
    }
  }
}

void GameWindow::drawPlayerPawns() {

  std::map<int, std::vector<int>> posMap;
  for (int i = 0; i < (int)state.players.size(); i++) {
    if (state.players[i].status != "BANKRUPT") {
      posMap[state.players[i].position].push_back(i);
    }
  }

  for (auto &[pos, indices] : posMap) {
    int safePos = (pos >= 1 && pos <= 40) ? pos : 1;
    Rectangle tRect = getTileRect(safePos);
    int n = (int)indices.size();
    for (int j = 0; j < n; j++) {
      int pidx = indices[j];
      Color pc = getPlayerColor(pidx);

      float ox = (j % 2) * 14.0f;
      float oy = (j / 2) * 14.0f;
      float px = tRect.x + tRect.width - 14 - ox;
      float py = tRect.y + tRect.height - 14 - oy;

      DrawCircle((int)px + 6, (int)py + 6, 7, pc);
      DrawCircleLines((int)px + 6, (int)py + 6, 7, C_BORDER);

      std::string num = std::to_string(pidx + 1);
      drawPixelText(num, (int)px + 3, (int)py + 2, 6, WHITE);
    }
  }
}

void GameWindow::drawSidebar() {
  DrawRectangleRec(sidebarRect, C_PANEL);
  DrawRectangleLinesEx(sidebarRect, 3, C_BORDER);

  int sx = (int)sidebarRect.x;
  int sy = (int)sidebarRect.y;

  DrawRectangle(sx, sy, (int)sidebarRect.width, 56, C_BTN_BG);
  drawPixelText("NIMONSPOLI", sx + 14, sy + 12, 9, C_BTN_TEXT);
  std::string turnStr = "TURN " + std::to_string(state.currentTurn) + "/" +
                        std::to_string(state.maxTurn);
  drawPixelText(turnStr, sx + 14, sy + 36, 5, {179, 166, 140, 255});

  int bannerY = sy + 56;
  DrawRectangle(sx, bannerY, (int)sidebarRect.width, 40, C_ACCENT_BG);
  std::string activeUser =
      state.players.empty() ? "?"
                            : state.players[state.currentPlayerIndex].username;
  std::string giliran = "GILIRAN: " + activeUser;

  Color ac = state.players.empty() ? C_ACCENT
                                   : getPlayerColor(state.currentPlayerIndex);
  DrawCircle(sx + 18, bannerY + 20, 7, ac);
  DrawCircleLines(sx + 18, bannerY + 20, 7, C_BORDER);
  drawPixelText(giliran, sx + 32, bannerY + 14, 6, C_ACCENT);

  DrawRectangle(sx, bannerY + 40, (int)sidebarRect.width, 2,
                {C_BORDER.r, C_BORDER.g, C_BORDER.b, 50});

  drawPlayerList();
  drawHandCards();
  drawLog();
}

void GameWindow::drawPlayerList() {
  int sx = (int)sidebarRect.x;
  int startY = (int)sidebarRect.y + 106;

  drawPixelText("PEMAIN:", sx + 14, startY, 6, C_TEXT_DIM);

  for (int i = 0; i < (int)state.players.size(); i++) {
    const PlayerInfo &p = state.players[i];
    int ry = startY + 16 + i * 44;

    bool isActive = (i == state.currentPlayerIndex);
    Color rowBg = isActive ? C_ACCENT_BG : C_PANEL_ALT;
    DrawRectangle(sx + 14, ry, (int)sidebarRect.width - 28, 38, rowBg);
    DrawRectangleLinesEx({(float)(sx + 14), (float)ry,
                          (float)((int)sidebarRect.width - 28), 38.0f},
                         isActive ? 2 : 1, C_BORDER);

    Color pc = getPlayerColor(i);
    DrawCircle(sx + 26, ry + 19, 6, pc);
    DrawCircleLines(sx + 26, ry + 19, 6, C_BORDER);

    std::string nameStr =
        "P" + std::to_string(i + 1) + " " + truncate(p.username, 8);
    Color nameColor = isActive ? C_ACCENT : C_TEXT;
    drawPixelText(nameStr, sx + 36, ry + 13, 6, nameColor);

    std::string moneyStr = formatMoney(p.money);
    drawPixelText(moneyStr, sx + 210, ry + 13, 6, C_GOOD);

    std::string posStr = "@" + std::to_string(p.position);
    drawPixelText(posStr, sx + 330, ry + 15, 5, C_TEXT_DIM);

    if (p.status == "JAILED") {
      DrawRectangle(sx + 150, ry + 8, 48, 14,
                    {C_DANGER.r, C_DANGER.g, C_DANGER.b, 180});
      drawPixelText("JAIL", sx + 154, ry + 11, 5, WHITE);
    } else if (p.status == "BANKRUPT") {
      DrawRectangle(sx + 150, ry + 8, 64, 14, {80, 80, 80, 180});
      drawPixelText("BANGKRUT", sx + 154, ry + 11, 4, WHITE);
    }
  }
}

void GameWindow::drawHandCards() {
  int sx = (int)sidebarRect.x;
  int startY =
      (int)sidebarRect.y + 106 + 16 + (int)state.players.size() * 44 + 8;

  DrawRectangle(sx, startY, (int)sidebarRect.width, 2,
                {C_BORDER.r, C_BORDER.g, C_BORDER.b, 50});
  startY += 6;

  drawPixelText("KARTU DI TANGAN:", sx + 14, startY, 6, C_TEXT_DIM);
  startY += 16;

  if (state.players.empty())
    return;
  const PlayerInfo &active = state.players[state.currentPlayerIndex];

  if (active.handCards.empty()) {
    drawPixelText("(kosong)", sx + 14, startY + 8, 5, C_TEXT_DIM);
    return;
  }

  static const Color cardColors[] = {
      {128, 51, 230, 255},
      {38, 140, 89, 255},
      {204, 128, 25, 255},
  };

  for (int i = 0; i < (int)active.handCards.size() && i < 3; i++) {
    const CardInfo &card = active.handCards[i];
    int ry = startY + i * 36;

    Color cc = cardColors[i % 3];

    DrawRectangle(sx + 14, ry, (int)sidebarRect.width - 28, 30, C_PANEL_ALT);
    DrawRectangleLinesEx({(float)(sx + 14), (float)ry,
                          (float)((int)sidebarRect.width - 28), 30.0f},
                         1, C_BORDER);

    DrawRectangle(sx + 14, ry, 5, 30, cc);

    drawPixelText(truncate(card.name, 12), sx + 24, ry + 11, 6, C_TEXT);

    drawPixelText(truncate(card.description, 18), sx + 160, ry + 12, 5,
                  C_TEXT_DIM);

    Rectangle useRect = {(float)(sx + (int)sidebarRect.width - 70),
                         (float)(ry + 4), 54.0f, 22.0f};
    bool hov = isHovered(useRect);
    drawButton("PAKAI", useRect, C_BTN_BG, C_BTN_TEXT, hov);

    if (hov && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      std::string cmdName = "GUNAKAN_KEMAMPUAN_" + std::to_string(i);
      auto it = commandCallbacks.find(cmdName);
      if (it != commandCallbacks.end())
        it->second();
    }
  }
}

void GameWindow::drawLog() {
  int sx = (int)sidebarRect.x;

  int numCards = 0;
  if (!state.players.empty()) {
    numCards = std::min(
        3, (int)state.players[state.currentPlayerIndex].handCards.size());
  }
  int cardsSectionH = numCards > 0 ? numCards * 36 : 20;
  int cardsEnd = (int)sidebarRect.y + 106 + 16 +
                 (int)state.players.size() * 44 + 8 + 2 + 6 + 16 +
                 cardsSectionH + 8;

  DrawRectangle(sx, cardsEnd, (int)sidebarRect.width, 2,
                {C_BORDER.r, C_BORDER.g, C_BORDER.b, 50});
  int startY = cardsEnd + 6;

  drawPixelText("LOG TERAKHIR:", sx + 14, startY, 6, C_TEXT_DIM);
  startY += 14;

  int showCount = std::min(5, (int)state.logs.size());
  int from = (int)state.logs.size() - showCount;

  for (int i = 0; i < showCount; i++) {
    const LogInfo &log = state.logs[from + i];
    int ry = startY + i * 28;

    bool isLatest = (i == showCount - 1);
    if (isLatest) {
      DrawRectangle(sx + 14, ry, (int)sidebarRect.width - 28, 24, C_ACCENT_BG);
    }

    std::string turnBadge = "T" + std::to_string(log.turn);
    drawPixelText(turnBadge, sx + 16, ry + 8, 5, C_ACCENT);

    std::string logText = truncate(log.username + " " + log.detail, 30);
    Color logColor = isLatest ? C_TEXT : C_TEXT_DIM;
    drawPixelText(logText, sx + 38, ry + 8, 5, logColor);
  }

  int btnY = (int)sidebarRect.y + (int)sidebarRect.height - 80;
  DrawRectangle(sx, btnY, (int)sidebarRect.width, 2,
                {C_BORDER.r, C_BORDER.g, C_BORDER.b, 50});
  Rectangle logBtnRect = {(float)(sx + 14), (float)(btnY + 8),
                          (float)((int)sidebarRect.width - 28), 26.0f};
  bool logHov = isHovered(logBtnRect);
  drawButton(">> CETAK LOG LENGKAP", logBtnRect, C_PANEL_ALT, C_TEXT_DIM,
             logHov);
  if (logHov && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    auto it = commandCallbacks.find("CETAK_LOG");
    if (it != commandCallbacks.end())
      it->second();
  }

  int saveBtnY = (int)sidebarRect.y + (int)sidebarRect.height - 46;
  Rectangle saveRect = {(float)(sx + 14), (float)saveBtnY, 192.0f, 32.0f};
  Rectangle loadRect = {(float)(sx + 218), (float)saveBtnY, 192.0f, 32.0f};
  bool saveHov = isHovered(saveRect);
  bool loadHov = isHovered(loadRect);

  drawButton("[ SIMPAN ]", saveRect, {245, 224, 178, 255}, C_WARN, saveHov);
  drawButton("[ MUAT ]", loadRect, C_PANEL_ALT, C_TEXT_DIM, loadHov);

  if (saveHov && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    auto it = commandCallbacks.find("SIMPAN");
    if (it != commandCallbacks.end())
      it->second();
  }
  if (loadHov && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    auto it = commandCallbacks.find("MUAT");
    if (it != commandCallbacks.end())
      it->second();
  }
}

void GameWindow::drawCommandBar() {
  DrawRectangleRec(cmdBarRect, C_PANEL);
  DrawRectangleLinesEx(cmdBarRect, 3, C_BORDER);

  int cx = (int)cmdBarRect.x;
  int cy = (int)cmdBarRect.y;

  drawPixelText("PERINTAH:", cx + 12, cy + 8, 6, C_TEXT_DIM);

  struct CmdBtn {
    std::string label;
    std::string cmd;
    Color bg;
    Color fg;
  };

  static const std::vector<CmdBtn> buttons = {
      {"LEMPAR\nDADU", "LEMPAR_DADU", C_BTN_BG, C_BTN_TEXT},
      {"BELI", "BELI", C_PANEL_ALT, C_TEXT},
      {"GADAI", "GADAI", C_PANEL_ALT, C_TEXT},
      {"TEBUS", "TEBUS", C_PANEL_ALT, C_TEXT},
      {"BANGUN", "BANGUN", C_PANEL_ALT, C_TEXT},
      {"CETAK\nPAPAN", "CETAK_PAPAN", C_PANEL_ALT, C_TEXT},
      {"CETAK\nAKTA", "CETAK_AKTA", C_PANEL_ALT, C_TEXT},
      {"CETAK\nPROPERTI", "CETAK_PROPERTI", C_PANEL_ALT, C_TEXT},
      {"AKHIRI\nGILIRAN", "AKHIRI_GILIRAN", {64, 20, 20, 255}, C_BTN_TEXT},
  };

  float gap = 8.0f;
  float btnH = 58.0f;
  float startX = cx + 12.0f;
  float btnY = cy + 24.0f;
  int numBtns = (int)buttons.size();
  float btnW = (CMD_W - 24.0f - (numBtns - 1) * gap) / (float)numBtns;

  for (int i = 0; i < (int)buttons.size(); i++) {
    const CmdBtn &b = buttons[i];
    Rectangle bRect = {startX + i * (btnW + gap), btnY, btnW, btnH};
    bool hov = isHovered(bRect);

    drawButton(b.label, bRect, b.bg, b.fg, hov);

    if (hov && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      auto it = commandCallbacks.find(b.cmd);
      if (it != commandCallbacks.end())
        it->second();
    }
  }
}

void GameWindow::drawPopup() {

  DrawRectangle(0, 0, screenW, screenH, {0, 0, 0, 140});

  float pw = 360.0f;
  float ph = 260.0f;
  float px = (screenW - pw) / 2.0f;
  float py = (screenH - ph) / 2.0f;
  Rectangle popRect = {px, py, pw, ph};

  DrawRectangleRec({px + 5, py + 5, pw, ph},
                   {C_BORDER.r, C_BORDER.g, C_BORDER.b, 100});

  DrawRectangleRec(popRect, C_PANEL);
  DrawRectangleLinesEx(popRect, 3, C_BORDER);

  Color headerColor = C_BTN_BG;
  if (popup.type == PopupType::BUY_PROPERTY)
    headerColor = {20, 46, 184, 255};
  if (popup.type == PopupType::AUCTION)
    headerColor = C_WARN;
  if (popup.type == PopupType::WINNER)
    headerColor = C_ACCENT;
  if (popup.type == PopupType::JAIL)
    headerColor = C_DANGER;

  DrawRectangle((int)px, (int)py, (int)pw, 44, headerColor);
  drawPixelText(popup.title, (int)px + 12, (int)py + 16, 7, WHITE);

  drawPixelText(popup.message, (int)px + 12, (int)py + 56, 5, C_TEXT);

  float optBtnH = 32.0f;
  float optBtnW = (pw - 28.0f - (popup.options.size() - 1) * 8.0f) /
                  std::max(1, (int)popup.options.size());

  for (int i = 0; i < (int)popup.options.size(); i++) {
    Rectangle optRect = {px + 14.0f + i * (optBtnW + 8.0f),
                         py + ph - optBtnH - 14.0f, optBtnW, optBtnH};

    bool hov = isHovered(optRect);
    Color bg = (i == 0) ? C_BTN_BG : C_PANEL_ALT;
    Color fg = (i == 0) ? C_BTN_TEXT : C_TEXT;
    drawButton(popup.options[i], optRect, bg, fg, hov);

    if (hov && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      popup.selectedIndex = i;
      if (popupCallback)
        popupCallback(i);
    }
  }

  drawPixelText("[ESC] tutup", (int)px + (int)pw - 90, (int)py + (int)ph - 16,
                4, C_TEXT_DIM);
  if (IsKeyPressed(KEY_ESCAPE)) {
    closePopup();
  }
}
