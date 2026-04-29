#include "../../include/views/MainMenuScene.hpp"
#include "../../include/views/SceneManager.hpp"
<<<<<<< HEAD
#include "../../include/core/GameManager.hpp"
#include "../../include/core/AccountManager.hpp"
#include "../../include/core/Game.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/core/Account.hpp"
#include "../../include/data-layer/ConfigComposer.hpp"
#include "../../include/utils/BoardBuilder.hpp"
#include "../../include/models/AbilityCard.hpp"
#include "raylib.h"
=======
#include "../../include/coredummy/IGameFacade.hpp"

>>>>>>> origin
#include <algorithm>
#include <cmath>
#include <cctype>
#include <string>
#include <vector>
<<<<<<< HEAD
#include <iostream>

namespace {
const Color kBgTop      {236,248,220,255};
const Color kBgBottom   {255,248,195,255};
const Color kPanel      {255,255,235,230};
const Color kPanelBorder{180,210,120,200};
const Color kAccent     {255,190, 30,255};
const Color kAccentAlt  { 90,180, 60,255};
const Color kText       { 50, 80, 20,255};
const Color kSubtext    {100,130, 50,255};
const Color kMuted      {160,185,110,255};
const Color kDanger     {220, 80, 60,255};
const Color kFlower1    {255,210, 50,255};

float easeT(float c, float t, float s) {
    return c + (t - c) * std::max(0.f, std::min(s, 1.f));
=======

namespace {
const Color kBgTop{236,248,220,255};
const Color kBgBottom{255,248,195,255};
const Color kPanel{255,255,235,230};
const Color kPanelBorder{180,210,120,200};
const Color kAccent{255,190,30,255};
const Color kAccentAlt{90,180,60,255};
const Color kText{50,80,20,255};
const Color kSubtext{100,130,50,255};
const Color kMuted{160,185,110,255};
const Color kDanger{220,80,60,255};
const Color kFlower{255,210,50,255};

float easeTowards(float current, float target, float speed) {
    float c = std::max(0.0f, std::min(speed, 1.0f));
    return current + (target - current) * c;
>>>>>>> origin
}

void drawFlower(float cx, float cy, float r, float angle, float alpha) {
    for (int i = 0; i < 5; ++i) {
<<<<<<< HEAD
        float a = angle + i * (6.283f / 5.f);
        DrawCircle(int(cx + r*1.3f*cosf(a)), int(cy + r*1.3f*sinf(a)), r, Fade(kFlower1, alpha));
    }
    DrawCircle(int(cx), int(cy), r*0.7f, Fade(kAccent, alpha));
    DrawCircle(int(cx), int(cy), r*0.35f, Fade({255,160,30,255}, alpha));
=======
        float a = angle + i * (6.28318f / 5.0f);
        DrawCircle(static_cast<int>(cx + r * 1.3f * std::cos(a)),
                   static_cast<int>(cy + r * 1.3f * std::sin(a)),
                   r, Fade(kFlower, alpha));
    }
    DrawCircle(static_cast<int>(cx), static_cast<int>(cy), r * 0.7f, Fade(kAccent, alpha));
    DrawCircle(static_cast<int>(cx), static_cast<int>(cy), r * 0.35f, Fade({255,160,30,255}, alpha));
}

std::string trimCopy(const std::string& value) {
    std::size_t first = 0;
    while (first < value.size() && std::isspace(static_cast<unsigned char>(value[first]))) {
        ++first;
    }
    std::size_t last = value.size();
    while (last > first && std::isspace(static_cast<unsigned char>(value[last - 1]))) {
        --last;
    }
    return value.substr(first, last - first);
}

std::string oneLineCopy(std::string value) {
    for (char& ch : value) {
        if (ch == '\n' || ch == '\r' || ch == '\t') {
            ch = ' ';
        }
    }
    return value;
}

std::string fitText(std::string value, int fontSize, int maxWidth) {
    value = oneLineCopy(value);
    if (MeasureText(value.c_str(), fontSize) <= maxWidth) {
        return value;
    }
    while (!value.empty() && MeasureText((value + "...").c_str(), fontSize) > maxWidth) {
        value.pop_back();
    }
    return value + "...";
>>>>>>> origin
}
}

MainMenuScene::MainMenuScene(SceneManager* sm, GameManager* gm, AccountManager* am)
    : Scene(sm, gm, am),
      configPathField("config/basic"),
      newGameButton("New Game", kAccent, kText),
      quitButton("Keluar", kDanger, {255,255,255,255}),
      startGameButton("Mulai!", kAccent, kText),
      cancelButton("Batal", kMuted, kText),
      plusButton("+", kAccentAlt, {255,255,255,255}),
      minusButton("-", kAccentAlt, {255,255,255,255}),
<<<<<<< HEAD
      showSetupModal(false), playerCount(4),
      sceneTime(0), modalVis(0) {

    configPathField.setContent("config/basic");
    playerFields.emplace_back("Pemain 1");
    playerFields.emplace_back("Pemain 2");
    playerFields.emplace_back("Pemain 3");
    playerFields.emplace_back("Pemain 4");

    newGameButton.setOnClick([this]() { showSetupModal = true; errorMsg.clear(); });
    quitButton.setOnClick([]() { CloseWindow(); });
    cancelButton.setOnClick([this]() { showSetupModal = false; });
    plusButton.setOnClick([this]()  { playerCount = std::min(4, playerCount + 1); });
    minusButton.setOnClick([this]() { playerCount = std::max(2, playerCount - 1); });
    startGameButton.setOnClick([this]() { onStartGame(); });
}

void MainMenuScene::onEnter() {
    sceneTime = 0; showSetupModal = false; modalVis = 0; errorMsg.clear();
}

void MainMenuScene::onStartGame() {
    // 1. Load config dari folder yang dipilih
    std::string folder = configPathField.getContent();
    if (folder.empty()) folder = "config/basic";

    try {
        ConfigComposer composer(
            folder + "/property.txt",
            folder + "/railroad.txt",
            folder + "/utility.txt",
            folder + "/tax.txt",
            folder + "/aksi.txt",
            folder + "/special.txt",
            folder + "/misc.txt"
        );

        // 2. Buat game baru via GameManager
        gameManager->startNewGame();
        Game* game = gameManager->getCurrentGame();
        if (!game) { errorMsg = "Gagal membuat game!"; return; }

        // 3. Set config ke game
        game->setConfig(composer.getConfig());

        // 4. Ambil saldo awal & max turn dari config
        int startMoney = game->getConfig().getMiscConfig(SALDO_AWAL);
        if (startMoney <= 0) startMoney = 1500;
        int maxTurn = game->getConfig().getMiscConfig(MAX_TURN);

        // 5. Buat pemain
        std::vector<Player>& players = game->getPlayers();
        players.clear();
        std::vector<int> order;

        for (int i = 0; i < playerCount; ++i) {
            std::string name = playerFields[size_t(i)].getContent();
            if (name.empty()) name = "Pemain " + std::to_string(i + 1);

            Account acc(name, "pass", 0);
            accountManager->addAccount(acc);
            Account* accPtr = accountManager->getAccount(name, "pass");
            players.emplace_back(i, accPtr, startMoney);
            order.push_back(i);
        }

        // 6. Set TurnManager
        game->getTurnManager() = TurnManager(order, maxTurn);

        // 7. Build board dari config
        BoardBuilder::build(
            game->getBoard(),
            composer.getConfig().getPropertyConfigAll(),
            composer.getConfig().getActionTileConfigAll(),
            [&](){
                std::map<int,int> m;
                Config& c = composer.getConfig();
                for(int i=1;i<=4;++i) m[i]=c.getRailroadRent(i);
                return m;
            }(),
            [&](){
                std::map<int,int> m;
                Config& c = composer.getConfig();
                for(int i=1;i<=2;++i) m[i]=c.getUtilityMultiplier(i);
                return m;
            }()
        );

        // 8. Switch ke InGameScene
        showSetupModal = false;
        sceneManager->setScene(SceneType::InGame);

    } catch (std::exception& e) {
        errorMsg = std::string("Error config: ") + e.what();
        std::cerr << errorMsg << std::endl;
    }
}

void MainMenuScene::layoutButtons(Rectangle sr) {
    float w = 190, h = 56;
    float y = sr.y + sr.height - 130;
    float left = sr.x + 72;
    newGameButton.setBoundary({left, y, w, h});
    quitButton.setBoundary({left + w + 16, y, 130, h});
}

void MainMenuScene::update() {
    float dt = GetFrameTime();
    sceneTime += dt;
    modalVis = easeT(modalVis, showSetupModal ? 1.f : 0.f, dt * 9.f);
    Rectangle sr{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()};
    layoutButtons(sr);
    if (IsKeyPressed(KEY_ESCAPE) && showSetupModal) showSetupModal = false;
    if (modalVis < 0.02f) {
=======
      configPathField("Folder Config"),
      showNewGameModal(false),
      playerCount(4),
      sceneTime(0.0f),
      modalVisibility(0.0f) {

    playerFields.emplace_back("Nama Pemain 1");
    playerFields.emplace_back("Nama Pemain 2");
    playerFields.emplace_back("Nama Pemain 3");
    playerFields.emplace_back("Nama Pemain 4");
    for (TextField& field : playerFields) {
        field.setMaxLength(8);
    }
    configPathField.setContent("config/basic");
    configPathField.setMaxLength(120);

    newGameButton.setOnClick([this]() {
        showNewGameModal = true;
        formError.clear();
        if (trimCopy(configPathField.getContent()).empty()) {
            configPathField.setContent("config/basic");
        }
    });
    loadDemoButton.setOnClick([this]() {
        gameFacade->loadDemoGame();
        sceneManager->setScene(SceneType::InGame);
    });
    quitButton.setOnClick([]() { CloseWindow(); });
    cancelButton.setOnClick([this]() {
        showNewGameModal = false;
        formError.clear();
    });
    plusButton.setOnClick([this]() {
        playerCount = std::min(4, playerCount + 1);
        formError.clear();
    });
    minusButton.setOnClick([this]() {
        playerCount = std::max(2, playerCount - 1);
        formError.clear();
    });
    startGameButton.setOnClick([this]() {
        std::vector<std::string> names;
        for (int i = 0; i < playerCount; ++i) {
            names.push_back(trimCopy(playerFields[static_cast<std::size_t>(i)].getContent()));
        }
        const std::string configDirectory = trimCopy(configPathField.getContent());
        formError = gameFacade->validateNewGameSettings(names, configDirectory);
        if (!formError.empty()) {
            return;
        }
        if (!gameFacade->startNewGame(names, configDirectory)) {
            formError = oneLineCopy(gameFacade->getViewModel().statusLine);
            if (formError.empty()) {
                formError = "Gagal membuat game baru.";
            }
            return;
        }
        showNewGameModal = false;
        formError.clear();
        sceneManager->setScene(SceneType::InGame);
    });
}

void MainMenuScene::onEnter() {
    sceneTime = 0.0f;
    showNewGameModal = false;
    modalVisibility = 0.0f;
    formError.clear();
}

void MainMenuScene::layoutButtons(Rectangle screenRect) {
    const float w = 190.0f;
    const float h = 56.0f;
    const float y = screenRect.y + screenRect.height - 130.0f;
    const float left = screenRect.x + 72.0f;
    const float gap = 16.0f;
    newGameButton.setBoundary({left, y, w, h});
    loadDemoButton.setBoundary({left + w + gap, y, w, h});
    quitButton.setBoundary({left + (w + gap) * 2.0f, y, 130.0f, h});
}

void MainMenuScene::update() {
    sceneTime += GetFrameTime();
    Rectangle screen{0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
    layoutButtons(screen);

    modalVisibility = easeTowards(modalVisibility, showNewGameModal ? 1.0f : 0.0f, GetFrameTime() * 9.0f);
    if (IsKeyPressed(KEY_ESCAPE) && showNewGameModal) {
        showNewGameModal = false;
        formError.clear();
    }

    if (modalVisibility < 0.02f) {
>>>>>>> origin
        newGameButton.update();
        quitButton.update();
        return;
    }
<<<<<<< HEAD
    configPathField.update();
    plusButton.update(); minusButton.update();
    startGameButton.update(); cancelButton.update();
    for (int i = 0; i < playerCount; ++i) playerFields[size_t(i)].update();
}

void MainMenuScene::drawBackground(Rectangle sr) {
    DrawRectangleGradientV(0,0,int(sr.width),int(sr.height), kBgTop, kBgBottom);
    struct F { float x,y,r,ph,sp; };
    F fs[] = {{.08f,.15f,28,.0f,.4f},{.92f,.1f,22,1.2f,.5f},{.05f,.75f,18,2.1f,.35f},
              {.95f,.8f,24,.8f,.45f},{.5f,.05f,16,1.5f,.6f},{.2f,.92f,20,3,.3f},
              {.8f,.9f,14,.5f,.55f},{.15f,.45f,12,2.5f,.7f},{.88f,.5f,15,1.8f,.42f}};
    for (auto& f : fs) {
        float a = sceneTime*f.sp+f.ph;
        float px = sr.x+f.x*sr.width, py = sr.y+f.y*sr.height;
        float bob = sinf(sceneTime*f.sp*1.5f+f.ph)*6.f;
        drawFlower(px,py+bob,f.r,a,0.55f);
        DrawLineEx({px,py+bob+f.r},{px,py+bob+f.r+f.r*2.5f},2.5f,Fade(kAccentAlt,0.4f));
    }
    for (int i = 0; i < 14; ++i) {
        float x = fmodf(80+i*105+sceneTime*(10+i*.5f),sr.width+60)-30;
        float y = 50+fmodf(i*67+sceneTime*(6+i*.3f),sr.height-100);
        DrawEllipse(int(x),int(y),5+i%3*3,(5+i%3*3)*.55f,Fade(kAccentAlt,0.18f));
    }
}

void MainMenuScene::drawHero(Rectangle sr) {
    float fy = sinf(sceneTime*1.1f)*7.f;
    DrawText("NIMONSPOLI",72,int(74+fy),76,kText);
    DrawRectangleRounded({72,156+fy,480,8},1,8,kAccent);
    DrawText("Permainan papan strategi khas Indonesia!",76,178,26,kSubtext);
    DrawText("Beli properti, bangun bangunan, jadilah orang terkaya!",76,210,22,kMuted);

    Rectangle pv{sr.width-510,74+fy,420,296};
    DrawRectangleRounded({pv.x+6,pv.y+10,pv.width,pv.height},.1f,12,Fade(kText,.08f));
    DrawRectangleRounded(pv,.1f,12,Fade(kPanel,.96f));
    DrawRectangleRoundedLinesEx(pv,.1f,12,2.5f,Fade(kPanelBorder,.9f));

    Rectangle bd{pv.x+20,pv.y+20,226,226};
    DrawRectangleRounded(bd,.04f,8,{235,245,215,255});
    DrawRectangleRoundedLinesEx(bd,.04f,8,3,kAccentAlt);
    float c = bd.width/11.f;
    for (int i = 0; i <= 11; ++i) {
        DrawLineEx({bd.x+i*c,bd.y},{bd.x+i*c,bd.y+bd.height},.8f,Fade(kPanelBorder,.4f));
        DrawLineEx({bd.x,bd.y+i*c},{bd.x+bd.width,bd.y+i*c},.8f,Fade(kPanelBorder,.4f));
    }

    Rectangle rib{bd.x+44,bd.y+74,138,72};
    DrawRectanglePro({rib.x+rib.width*.5f,rib.y+rib.height*.5f,rib.width,rib.height},
                     {rib.width*.5f,rib.height*.5f},-22,kAccent);
    DrawText("NIMONSPOLI",int(bd.x+50),int(bd.y+97),20,kText);

    float orb = sceneTime*1.1f;
    Color tc[] = {kAccent,kAccentAlt,{255,120,80,255},{80,160,220,255}};
    for (int i = 0; i < 4; ++i) {
        float px = bd.x+40+i*44+cosf(orb+i)*5;
        float py = bd.y+185+sinf(orb*1.3f+i*.7f)*8;
        DrawCircle(int(px),int(py),10,tc[i]);
    }

    Rectangle si{pv.x+260,pv.y+20,128,252};
    DrawRectangleRounded(si,.14f,10,{240,255,220,255});
    DrawText("Fitur",int(si.x+14),int(si.y+14),22,kText);
    const char* ft[] = {"40 petak","4 pemain","Lelang","Festival","Kartu skill"};
    for (int i = 0; i < 5; ++i) DrawText(ft[i],int(si.x+14),int(si.y+50+i*36),18,kSubtext);
}

void MainMenuScene::drawFeatureCards(Rectangle sr) {
    struct C { const char* t; const char* l[2]; Color c; };
    C cs[] = {{"Properti",{"Beli & bangun","rumah & hotel"},kAccent},
              {"Strategi",{"Gadai, lelang,","festival sewa"},kAccentAlt},
              {"Kartu",{"Skill card &","efek spesial"},{255,160,30,255}}};
    float sx = 72, y = 420, gap = 18;
    float cw = (sr.width-sx*2-gap*2)/3;
    for (int i = 0; i < 3; ++i) {
        float bob = sinf(sceneTime*1.4f+i*.9f)*6;
        Rectangle r{sx+i*(cw+gap),y+bob,cw,158};
        DrawRectangleRounded({r.x+4,r.y+8,r.width,r.height},.16f,10,Fade(kText,.07f));
        DrawRectangleRounded(r,.16f,10,Fade(kPanel,.95f));
        DrawRectangleRoundedLinesEx(r,.16f,10,2,Fade(cs[i].c,.6f));
        DrawRectangleRounded({r.x+14,r.y+14,80,10},1,8,cs[i].c);
        drawFlower(r.x+r.width-30,r.y+28,9,sceneTime*.4f+i,.6f);
        DrawText(cs[i].t,int(r.x+14),int(r.y+42),28,kText);
        DrawText(cs[i].l[0],int(r.x+14),int(r.y+92),20,kSubtext);
        DrawText(cs[i].l[1],int(r.x+14),int(r.y+116),20,kSubtext);
=======

    plusButton.update();
    minusButton.update();
    startGameButton.update();
    cancelButton.update();
    for (int i = 0; i < playerCount; ++i) {
        playerFields[static_cast<std::size_t>(i)].update();
    }
    configPathField.update();
}

void MainMenuScene::drawBackground(Rectangle screenRect) {
    DrawRectangleGradientV(0, 0, static_cast<int>(screenRect.width), static_cast<int>(screenRect.height),
                           kBgTop, kBgBottom);
    for (int i = 0; i < 9; ++i) {
        float x = screenRect.x + (0.08f + 0.105f * i) * screenRect.width;
        float y = screenRect.y + (0.15f + 0.08f * (i % 5)) * screenRect.height;
        drawFlower(x, y + std::sin(sceneTime * 0.8f + i) * 7.0f, 12.0f + (i % 3) * 5.0f,
                   sceneTime * 0.4f + i, 0.35f);
    }
}

void MainMenuScene::drawHero(Rectangle) {
    float bob = std::sin(sceneTime * 1.1f) * 7.0f;
    DrawText("NIMONSPOLI", 72, static_cast<int>(74 + bob), 76, kText);
    DrawRectangleRounded({72, 156 + bob, 480, 8}, 1.0f, 8, kAccent);
    DrawText("Permainan papan strategi khas Indonesia!", 76, 178, 26, kSubtext);
    DrawText("Beli properti, bangun bangunan, jadilah orang terkaya!", 76, 210, 22, kMuted);
}

void MainMenuScene::drawFeatureCards(Rectangle screenRect) {
    struct Card { const char* title; const char* line1; const char* line2; Color color; };
    const Card cards[] = {
        {"Properti", "Beli & bangun", "rumah & hotel", kAccent},
        {"Strategi", "Gadai, lelang,", "festival sewa", kAccentAlt},
        {"Kartu", "Skill card &", "efek spesial", {255,160,30,255}},
    };
    float sx = 72.0f;
    float y = 420.0f;
    float gap = 18.0f;
    float cw = (screenRect.width - sx * 2.0f - gap * 2.0f) / 3.0f;
    for (int i = 0; i < 3; ++i) {
        Rectangle r{sx + i * (cw + gap), y + std::sin(sceneTime * 1.4f + i) * 6.0f, cw, 158.0f};
        DrawRectangleRounded({r.x + 4, r.y + 8, r.width, r.height}, 0.16f, 10, Fade(kText, 0.07f));
        DrawRectangleRounded(r, 0.16f, 10, Fade(kPanel, 0.95f));
        DrawRectangleRoundedLinesEx(r, 0.16f, 10, 2.0f, Fade(cards[i].color, 0.6f));
        DrawRectangleRounded({r.x + 14, r.y + 14, 80, 10}, 1.0f, 8, cards[i].color);
        DrawText(cards[i].title, static_cast<int>(r.x + 14), static_cast<int>(r.y + 42), 28, kText);
        DrawText(cards[i].line1, static_cast<int>(r.x + 14), static_cast<int>(r.y + 92), 20, kSubtext);
        DrawText(cards[i].line2, static_cast<int>(r.x + 14), static_cast<int>(r.y + 116), 20, kSubtext);
>>>>>>> origin
    }
}

void MainMenuScene::drawSetupModal(Rectangle sr) {
    if (modalVis <= 0.01f) return;
    DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),Fade(kText,.35f*modalVis));
    float rise = (1-modalVis)*28;
    Rectangle m{sr.width*.5f-300, sr.height*.5f-270+rise, 600, 540};

<<<<<<< HEAD
    DrawRectangleRounded({m.x+6,m.y+10,m.width,m.height},.1f,12,Fade(kText,.1f*modalVis));
    DrawRectangleRounded(m,.1f,12,Fade({250,255,235,255},modalVis));
    DrawRectangleRoundedLinesEx(m,.1f,12,2.5f,Fade(kPanelBorder,modalVis));

    drawFlower(m.x+m.width-30,m.y+30,16,sceneTime*.5f,.5f*modalVis);
    drawFlower(m.x+30,m.y+30,12,sceneTime*.4f+1,.4f*modalVis);

    DrawText("Atur Permainan",int(m.x+26),int(m.y+24),34,kText);

    // Config folder
    DrawText("Folder Config:",int(m.x+26),int(m.y+72),22,kText);
    configPathField.setBoundary({m.x+26, m.y+98, m.width-52, 48});
    configPathField.draw();

    // Error message
    if (!errorMsg.empty())
        DrawText(errorMsg.c_str(),int(m.x+26),int(m.y+152),16,kDanger);

    // Player count
    DrawText("Jumlah Pemain:",int(m.x+26),int(m.y+178),22,kText);
    minusButton.setBoundary({m.x+196, m.y+168, 52, 42});
    plusButton.setBoundary({m.x+320, m.y+168, 52, 42});
    DrawRectangleRounded({m.x+256, m.y+168, 56, 42},.24f,8,Fade(kAccent,.15f));
    DrawText(std::to_string(playerCount).c_str(),int(m.x+279),int(m.y+178),24,kText);
    plusButton.draw(); minusButton.draw();

    // Player name fields
    for (int i = 0; i < 4; ++i) {
        float fy = m.y + 228 + i * 56;
        playerFields[size_t(i)].setBoundary({m.x+26, fy, m.width-52, 44});
        playerFields[size_t(i)].draw();
        if (i >= playerCount) {
            DrawRectangleRounded({m.x+26, fy, m.width-52, 44},.22f,8,Fade(kText,.3f));
            DrawText("tidak digunakan",int(m.x+m.width-180),int(fy+12),18,Fade(kMuted,.9f));
        }
    }

    startGameButton.setBoundary({m.x+m.width-220, m.y+m.height-70, 156, 50});
    cancelButton.setBoundary({m.x+m.width-390, m.y+m.height-70, 140, 50});
    startGameButton.draw(); cancelButton.draw();
}

void MainMenuScene::draw() {
    Rectangle sr{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()};
    drawBackground(sr);
    drawHero(sr);
    drawFeatureCards(sr);
    DrawText("Versi Indonesia dari permainan papan klasik dunia.",76,int(sr.height-208),24,kText);
    DrawText("Strategi, keberuntungan, dan kecerdasan finansial.",76,int(sr.height-172),22,kSubtext);
=======
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(kText, 0.35f * modalVisibility));
    Rectangle modal{screenRect.width * 0.5f - 300.0f,
                    screenRect.height * 0.5f - 280.0f + (1.0f - modalVisibility) * 28.0f,
                    600.0f, 560.0f};
    DrawRectangleRounded({modal.x + 6, modal.y + 10, modal.width, modal.height}, 0.1f, 12, Fade(kText, 0.1f));
    DrawRectangleRounded(modal, 0.1f, 12, Fade({250,255,235,255}, modalVisibility));
    DrawRectangleRoundedLinesEx(modal, 0.1f, 12, 2.5f, Fade(kPanelBorder, modalVisibility));
    DrawText("Atur Pemain", static_cast<int>(modal.x + 26), static_cast<int>(modal.y + 24), 34, kText);

    DrawText("Jumlah Pemain:", static_cast<int>(modal.x + 26), static_cast<int>(modal.y + 84), 22, kText);
    minusButton.setBoundary({modal.x + 196, modal.y + 74, 52, 42});
    plusButton.setBoundary({modal.x + 320, modal.y + 74, 52, 42});
    DrawRectangleRounded({modal.x + 256, modal.y + 74, 56, 42}, 0.24f, 8, Fade(kAccent, 0.15f));
    DrawText(std::to_string(playerCount).c_str(), static_cast<int>(modal.x + 279), static_cast<int>(modal.y + 84), 24, kText);
    plusButton.draw();
    minusButton.draw();

    for (int i = 0; i < 4; ++i) {
        float fy = modal.y + 140 + i * 56;
        playerFields[static_cast<std::size_t>(i)].setBoundary({modal.x + 26, fy, modal.width - 52, 44});
        playerFields[static_cast<std::size_t>(i)].draw();
        if (i >= playerCount) {
            DrawRectangleRounded({modal.x + 26, fy, modal.width - 52, 44}, 0.22f, 8, Fade(kText, 0.3f));
            DrawText("tidak digunakan", static_cast<int>(modal.x + modal.width - 180), static_cast<int>(fy + 12), 18, Fade(kMuted, 0.9f));
        }
    }

    DrawText("Folder Config:", static_cast<int>(modal.x + 26), static_cast<int>(modal.y + 374), 20, kText);
    configPathField.setBoundary({modal.x + 26, modal.y + 402, modal.width - 52, 44});
    configPathField.draw();

    if (!formError.empty()) {
        const std::string shownError = fitText(formError, 18, static_cast<int>(modal.width - 52));
        DrawText(shownError.c_str(), static_cast<int>(modal.x + 26), static_cast<int>(modal.y + 456), 18, kDanger);
    }

    startGameButton.setBoundary({modal.x + modal.width - 220, modal.y + modal.height - 70, 156, 50});
    cancelButton.setBoundary({modal.x + modal.width - 390, modal.y + modal.height - 70, 140, 50});
    startGameButton.draw();
    cancelButton.draw();
}

void MainMenuScene::draw() {
    Rectangle screen{0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
    drawBackground(screen);
    drawHero(screen);
    drawFeatureCards(screen);
    DrawText("Versi Indonesia dari permainan papan klasik dunia.", 76, static_cast<int>(screen.height - 208), 24, kText);
    DrawText("Strategi, keberuntungan, dan kecerdasan finansial.", 76, static_cast<int>(screen.height - 172), 22, kSubtext);
>>>>>>> origin
    newGameButton.draw();
    quitButton.draw();
<<<<<<< HEAD
    drawSetupModal(sr);
=======
    drawNewGameModal(screen);
>>>>>>> origin
}
