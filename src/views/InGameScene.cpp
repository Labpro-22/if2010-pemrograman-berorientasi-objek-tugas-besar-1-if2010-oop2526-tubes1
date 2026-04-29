#include "../../include/views/InGameScene.hpp"
<<<<<<< HEAD
#include "../../include/views/SceneManager.hpp"
#include "../../include/core/GameManager.hpp"
#include "../../include/core/Game.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/AbilityCard.hpp"
#include "../../include/utils/Board.hpp"
#include "../../include/utils/Tile.hpp"
#include "../../include/utils/PropertyTile.hpp"
#include "../../include/utils/StreetTile.hpp"
#include "../../include/utils/RailroadTile.hpp"
#include "../../include/utils/UtilityTile.hpp"
#include "../../include/utils/LogEntry.hpp"
#include "../../include/core/TurnManager.hpp"
#include "../../include/utils/Logger.hpp"
#include "raylib.h"
=======
#include "../../include/coredummy/GameViewModel.hpp"
#include "../../include/coredummy/IGameFacade.hpp"
#include "../../include/views/SceneManager.hpp"

>>>>>>> origin
#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <string>
#include <vector>

namespace {
<<<<<<< HEAD
const Color kBgA{230,245,210,255}, kBgB{255,248,195,255};
const Color kPanel{245,255,230,230}, kPanelBorder{160,200,100,180};
const Color kBoardSurf{240,248,220,255}, kCenterSurf{225,242,200,255};
const Color kText{45,75,15,255}, kSubtext{90,130,45,255};
const Color kAccent{255,190,30,255}, kAccentAlt{80,175,50,255};
const Color kDanger{210,70,50,255};
const std::array<Color,4> kTok = {Color{255,190,30,255},{80,175,50,255},{255,120,60,255},{60,160,220,255}};

Color groupCol(const std::string& g) {
    if(g=="COKLAT")return{135,86,58,255}; if(g=="BIRU_MUDA")return{124,215,255,255};
    if(g=="MERAH_MUDA")return{245,120,182,255}; if(g=="ORANGE")return{244,154,74,255};
    if(g=="MERAH")return{228,77,75,255}; if(g=="KUNING")return{241,213,81,255};
    if(g=="HIJAU")return{88,191,120,255}; if(g=="BIRU_TUA")return{65,92,202,255};
    if(g=="ABU"||g=="ABU_ABU")return{149,158,176,255};
    return{200,210,185,255};
=======
const Color kBgA{230,245,210,255};
const Color kBgB{255,248,195,255};
const Color kPanel{245,255,230,230};
const Color kPanelBorder{160,200,100,180};
const Color kBoardSurf{240,248,220,255};
const Color kCenterSurf{225,242,200,255};
const Color kText{45,75,15,255};
const Color kSubtext{90,130,45,255};
const Color kAccent{255,190,30,255};
const Color kAccentAlt{80,175,50,255};
const Color kDanger{210,70,50,255};
const std::array<Color,4> kTokens = {
    Color{255,190,30,255},
    Color{80,175,50,255},
    Color{255,120,60,255},
    Color{60,160,220,255},
};

Color groupColor(const std::string& group) {
    if (group == "COKLAT") return {135,86,58,255};
    if (group == "BIRU_MUDA") return {124,215,255,255};
    if (group == "MERAH_MUDA") return {245,120,182,255};
    if (group == "ORANGE") return {244,154,74,255};
    if (group == "MERAH") return {228,77,75,255};
    if (group == "KUNING") return {241,213,81,255};
    if (group == "HIJAU") return {88,191,120,255};
    if (group == "BIRU_TUA") return {65,92,202,255};
    if (group == "ABU" || group == "ABU_ABU") return {149,158,176,255};
    return {200,210,185,255};
>>>>>>> origin
}
float ease(float c,float t,float r){return c+(t-c)*std::max(0.f,std::min(r,1.f));}

<<<<<<< HEAD
void drawSmallFlower(float cx,float cy,float r,float a,float al){
    for(int i=0;i<5;++i){float an=a+i*(6.283f/5.f);
        DrawCircle(int(cx+r*1.3f*cosf(an)),int(cy+r*1.3f*sinf(an)),r,Fade(kAccent,al));}
    DrawCircle(int(cx),int(cy),r*.6f,Fade({255,160,30,255},al));
=======
std::string statusText(PropertyStatusView status) {
    switch (status) {
        case PropertyStatusView::Bank: return "BANK";
        case PropertyStatusView::Owned: return "OWNED";
        case PropertyStatusView::Mortgaged: return "GADAI";
    }
    return "";
}

float ease(float current, float target, float speed) {
    float clamped = std::max(0.0f, std::min(speed, 1.0f));
    return current + (target - current) * clamped;
}

void drawSmallFlower(float cx, float cy, float r, float angle, float alpha) {
    for (int i = 0; i < 5; ++i) {
        float a = angle + i * (6.28318f / 5.0f);
        DrawCircle(static_cast<int>(cx + r * 1.3f * std::cos(a)),
                   static_cast<int>(cy + r * 1.3f * std::sin(a)),
                   r, Fade(kAccent, alpha));
    }
    DrawCircle(static_cast<int>(cx), static_cast<int>(cy), r * 0.6f, Fade({255,160,30,255}, alpha));
>>>>>>> origin
}
}

InGameScene::InGameScene(SceneManager* sm, GameManager* gm, AccountManager* am)
    : Scene(sm,gm,am),
      closeOverlayBtn("X",kDanger,{255,255,255,255}),
      backToMenuBtn("Menu",kSubtext,{255,255,255,255}),
      sceneTime(0), selectedTile(0), overlayOpen(false), overlayVis(0) {

<<<<<<< HEAD
    struct Spec { const char* label; std::function<void()> fn; };
    std::vector<Spec> specs = {
        {"Lempar Dadu",[this](){
            Game* g = gameManager->getCurrentGame(); if(!g)return;
            auto r = g->getDice().roll(); int d1=r.first,d2=r.second;
            int idx = g->getTurnManager().getCurrentPlayerIndex();
            Player& p = g->getPlayer(idx);
            bool passGo = false;
            if(g->getBoard().size()==0)return;
            int np = g->getBoard().calculateNewPosition(p.getPosition(),d1+d2,passGo);
            p.moveTo(np);
            if(passGo){int sal=g->getConfig().getSpecialConfig(GO_SALARY);p.receive(sal);
                g->getLogger().log(g->getTurnManager().getCurrentTurn(),p.getUsername(),"GO","M"+std::to_string(sal));}
            g->handleLanding(p);
            g->getLogger().log(g->getTurnManager().getCurrentTurn(),p.getUsername(),"DADU",
                std::to_string(d1)+"+"+std::to_string(d2)+"="+std::to_string(d1+d2));
        }},
        {"Info Petak",[this](){
            Game* g = gameManager->getCurrentGame(); if(!g)return;
            Tile* t = g->getBoard().getTileByIndex(selectedTile);
            if(!t)return;
            std::vector<std::string> lines = {"Kode: "+t->getCode(),"Nama: "+t->getName()};
            PropertyTile* pt = dynamic_cast<PropertyTile*>(t);
            if(pt){lines.push_back("Harga: M"+std::to_string(pt->getLandPrice()));
                lines.push_back("Pemilik: "+(pt->getOwner()?pt->getOwner()->getUsername():"BANK"));
                StreetTile* st=dynamic_cast<StreetTile*>(pt);
                if(st){lines.push_back("Grup: "+st->getColorGroup());
                    lines.push_back("Rumah: "+std::to_string(st->getHouseCount())+(st->hasHotelBuilt()?" (Hotel)":""));}}
            showOverlay(t->getName(),lines);
        }},
        {"Properti",[this](){
            Game* g = gameManager->getCurrentGame(); if(!g)return;
            int idx=g->getTurnManager().getCurrentPlayerIndex();
            Player& p = g->getPlayer(idx);
            std::vector<std::string> lines = {"Pemilik: "+p.getUsername()};
            for(PropertyTile* pt:p.getOwnedProperties()) lines.push_back(pt->getCode()+" - "+pt->getName()+(pt->isMortgaged()?" [GADAI]":""));
            if(lines.size()==1) lines.push_back("(tidak ada)");
            showOverlay("Properti "+p.getUsername(),lines);
        }},
        {"Kartu",[this](){
            Game* g = gameManager->getCurrentGame(); if(!g)return;
            int idx=g->getTurnManager().getCurrentPlayerIndex();
            Player& p = g->getPlayer(idx);
            std::vector<std::string> lines;
            for(const auto& c:p.getHandCards()) lines.push_back(c->getName()+" - "+c->getDescription());
            if(lines.empty()) lines.push_back("(tidak ada kartu)");
            showOverlay("Kartu Kemampuan",lines);
        }},
        {"Akhir Giliran",[this](){
            Game* g = gameManager->getCurrentGame(); if(!g)return;
            g->endTurn();
        }},
    };
    for(auto& s:specs){Button b(s.label,kAccentAlt,{255,255,255,255});b.setOnClick(s.fn);actionButtons.push_back(b);}
    closeOverlayBtn.setOnClick([this](){overlayOpen=false;});
    backToMenuBtn.setOnClick([this](){sceneManager->setScene(SceneType::MainMenu);});
}

void InGameScene::onEnter(){sceneTime=0;overlayVis=0;overlayOpen=false;tokenPos.clear();tokenPhase.clear();selectedTile=0;}
void InGameScene::showOverlay(const std::string& t,const std::vector<std::string>& l){overlayTitle=t;overlayLines=l;overlayOpen=true;}

void InGameScene::layoutUi(Rectangle sr,Rectangle& br,Rectangle& sb){
    float sm=20,tm=88,gap=16;
    float sw=std::max(330.f,std::min(sr.width*.28f,380.f));
    float bs=std::min(sr.height-tm-24,sr.width-sw-gap-sm*2);
    br={sm,tm,bs,bs}; sb={br.x+br.width+gap,tm,sw,bs};
}

Rectangle InGameScene::getTileRect(const Rectangle& br,int i) const {
    float c=br.width/11.f;
    if(i>=0&&i<=10)return{br.x+br.width-(i+1.f)*c,br.y+br.height-c,c,c};
    if(i>=11&&i<=19)return{br.x,br.y+br.height-(i-10+1.f)*c,c,c};
    if(i>=20&&i<=30)return{br.x+(i-20.f)*c,br.y,c,c};
    return{br.x+br.width-c,br.y+(i-30.f)*c,c,c};
}
Vector2 InGameScene::getTileCenter(const Rectangle& br,int i) const{auto r=getTileRect(br,i);return{r.x+r.width*.5f,r.y+r.height*.5f};}

void InGameScene::updateAnimations(const Rectangle& br){
    Game* g = gameManager->getCurrentGame(); if(!g)return;
    size_t pc = g->getPlayers().size();
    if(tokenPos.size()!=pc){tokenPos.assign(pc,{0,0});tokenPhase.assign(pc,0);
        for(size_t i=0;i<pc;++i){tokenPos[i]=getTileCenter(br,g->getPlayers()[i].getPosition());tokenPhase[i]=i*1.37f;}}
    float dt=GetFrameTime();
    for(size_t i=0;i<pc;++i){
        Vector2 tgt=getTileCenter(br,g->getPlayers()[i].getPosition());
        tokenPos[i].x=ease(tokenPos[i].x,tgt.x,dt*6);tokenPos[i].y=ease(tokenPos[i].y,tgt.y,dt*6);
        tokenPhase[i]+=dt*(1.1f+i*.12f);}
    overlayVis=ease(overlayVis,overlayOpen?1.f:0.f,dt*8);
}

void InGameScene::update(){
    sceneTime+=GetFrameTime();
    Rectangle sr{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},br{},sb{};
    layoutUi(sr,br,sb);
    tileRects.clear();
    Game* g = gameManager->getCurrentGame();
    int tileCount = g ? g->getBoard().size() : 0;
    for(int i=0;i<tileCount;++i) tileRects.push_back(getTileRect(br,i));
    updateAnimations(br);
    if(IsKeyPressed(KEY_ESCAPE)){if(overlayOpen)overlayOpen=false;else{sceneManager->setScene(SceneType::MainMenu);return;}}
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){Vector2 m=GetMousePosition();
        for(size_t i=0;i<tileRects.size();++i) if(CheckCollisionPointRec(m,tileRects[i])){selectedTile=int(i);break;}}
    backToMenuBtn.setBoundary({sr.width-130,22,106,42});backToMenuBtn.update();
    float bw=(sb.width-14)*.5f,bh=44,sy=sb.y+238;
    for(size_t i=0;i<actionButtons.size();++i){
        int r=int(i)/2,c=int(i)%2;
        actionButtons[i].setBoundary({sb.x+c*(bw+14),sy+r*54,bw,bh});actionButtons[i].update();}
    if(overlayVis>.01f){closeOverlayBtn.setBoundary({sr.width*.5f+268,sr.height*.5f-240,50,38});closeOverlayBtn.update();}
}

void InGameScene::drawBackground(Rectangle sr){
    DrawRectangleGradientV(0,0,int(sr.width),int(sr.height),kBgA,kBgB);
    drawSmallFlower(40,40,18,sceneTime*.3f,.3f);
    drawSmallFlower(sr.width-50,50,14,sceneTime*.35f+1,.25f);
    drawSmallFlower(30,sr.height-50,12,sceneTime*.4f+2,.25f);
    drawSmallFlower(sr.width-40,sr.height-40,16,sceneTime*.3f+.5f,.25f);
    for(int i=0;i<10;++i){float x=fmodf(60+i*143+sceneTime*(8+i*.4f),sr.width+50)-25;
        float y=40+fmodf(i*73+sceneTime*(5+i*.25f),sr.height-80);
        DrawEllipse(int(x),int(y),4+i%3,2+i%2,Fade(kAccentAlt,.15f));}
}

void InGameScene::drawHeader(Rectangle sr){
    Game* g = gameManager->getCurrentGame();
    DrawText("Nimonspoli",22,16,34,kText);
    if(g){
        int ci=g->getTurnManager().getCurrentPlayerIndex();
        std::string status = (ci>=0&&ci<(int)g->getPlayers().size()) ? "Giliran: "+g->getPlayers()[ci].getUsername() : "";
        DrawText(status.c_str(),22,52,20,kSubtext);
        Rectangle badge{380,18,240,46};
        DrawRectangleRounded(badge,.28f,8,Fade(kAccent,.2f));
        DrawRectangleRoundedLinesEx(badge,.28f,8,1.5f,Fade(kPanelBorder,.8f));
        std::string tt="Turn "+std::to_string(g->getTurnManager().getCurrentTurn())+" / "+std::to_string(g->getTurnManager().getMaxTurn());
        DrawText(tt.c_str(),int(badge.x+20),int(badge.y+12),22,kText);
    }
    backToMenuBtn.draw();
}

void InGameScene::drawCenterPanel(const Rectangle& br){
    float c=br.width/11.f;
    Rectangle ctr{br.x+c,br.y+c,br.width-2*c,br.height-2*c};
    DrawRectangleRounded(ctr,.05f,10,kCenterSurf);
    DrawRectangleRoundedLinesEx(ctr,.05f,10,3,Fade(kAccentAlt,.6f));
    Rectangle rib{ctr.x+ctr.width*.25f,ctr.y+ctr.height*.42f,ctr.width*.5f,68};
    DrawRectanglePro({rib.x+rib.width*.5f,rib.y+rib.height*.5f,rib.width,rib.height},{rib.width*.5f,rib.height*.5f},-22,kAccent);
    DrawText("NIMONSPOLI",int(ctr.x+ctr.width*.27f),int(ctr.y+ctr.height*.43f),38,kText);
    drawSmallFlower(ctr.x+ctr.width*.5f,ctr.y+ctr.height*.72f,20,sceneTime*.6f,.4f);
}

void InGameScene::drawBoard(const Rectangle& br){
    Game* g = gameManager->getCurrentGame(); if(!g)return;
    DrawRectangleRounded({br.x+5,br.y+7,br.width,br.height},.04f,10,Fade(kText,.08f));
    DrawRectangleRounded(br,.04f,10,kBoardSurf);
    DrawRectangleRoundedLinesEx(br,.04f,10,3,Fade(kAccentAlt,.7f));

    if(g->getBoard().size()==0) { drawCenterPanel(br); return; }
    const auto& tiles = g->getBoard().getTiles();
    for(size_t i=0;i<tiles.size()&&i<tileRects.size();++i){
        Tile* tile=tiles[i]; if(!tile)continue;
        Rectangle r=tileRects[i];
        bool sel=(int(i)==selectedTile);
        DrawRectangleRec(r,kBoardSurf);
        DrawRectangleLinesEx(r,1,Fade(kPanelBorder,.5f));

        // Color band
        std::string cg="DEFAULT";
        StreetTile* st=dynamic_cast<StreetTile*>(tile);
        if(st) cg=st->getColorGroup();
        Rectangle band=r;
        if(i<=10){band.height=16;band.y=r.y+r.height-band.height;}
        else if(i<=19){band.width=16;}
        else if(i<=30){band.height=16;}
        else{band.width=16;band.x=r.x+r.width-band.width;}
        DrawRectangleRec(band,groupCol(cg));

        if(sel){float pu=.5f+.5f*sinf(sceneTime*3);DrawRectangleLinesEx(r,3+pu*2,kAccent);}
        DrawText(tile->getCode().c_str(),int(r.x+5),int(r.y+5),16,kText);
        DrawText(tile->getName().c_str(),int(r.x+5),int(r.y+25),10,kSubtext);

        PropertyTile* pt=dynamic_cast<PropertyTile*>(tile);
        if(pt&&pt->getOwner()){
            int oi=0; for(auto& p:g->getPlayers()){if(&p==pt->getOwner())break;oi++;}
            DrawCircle(int(r.x+r.width-14),int(r.y+14),7,kTok[oi%4]);}
        if(pt&&pt->isMortgaged()) DrawText("G",int(r.x+r.width-22),int(r.y+26),14,kDanger);
        if(st){for(int h=0;h<st->getHouseCount()&&h<4;++h) DrawRectangle(int(r.x+7+h*11),int(r.y+r.height-34),8,8,kAccentAlt);
            if(st->hasHotelBuilt()) DrawRectangle(int(r.x+7),int(r.y+r.height-34),16,10,kDanger);}
        if(pt&&pt->getFestivalMultiplier()>1){std::string fs="x"+std::to_string(pt->getFestivalMultiplier());
            DrawText(fs.c_str(),int(r.x+r.width-26),int(r.y+r.height-22),13,{180,50,200,255});}
    }

    drawCenterPanel(br);

    // Tokens
    for(size_t i=0;i<tokenPos.size();++i){
        float bob=sinf(tokenPhase[i]*2)*4;
        Vector2 pos{tokenPos[i].x,tokenPos[i].y+bob};
        Color tint=kTok[i%4];
        DrawCircleLines(int(pos.x),int(pos.y),14+sinf(tokenPhase[i]*2.5f)*2,Fade(tint,.3f));
        DrawCircle(int(pos.x),int(pos.y),10,tint);
        DrawCircleLines(int(pos.x),int(pos.y),10,Fade(WHITE,.5f));
        DrawText(std::to_string(int(i)+1).c_str(),int(pos.x-4),int(pos.y-6),12,kText);
    }
}

void InGameScene::drawSidebar(const Rectangle& sb){
    Game* g = gameManager->getCurrentGame(); if(!g||g->getPlayers().empty())return;

    DrawRectangleRounded({sb.x+4,sb.y+7,sb.width,sb.height},.04f,10,Fade(kText,.07f));
    DrawRectangleRounded(sb,.04f,10,Fade(kPanel,.97f));
    DrawRectangleRoundedLinesEx(sb,.04f,10,2,Fade(kPanelBorder,.8f));
    drawSmallFlower(sb.x+sb.width-22,sb.y+22,9,sceneTime*.4f,.35f);

    int ci=g->getTurnManager().getCurrentPlayerIndex();
    if(ci<0||ci>=(int)g->getPlayers().size())return;
    Player& cur=g->getPlayer(ci);
    DrawText("Giliran Sekarang",int(sb.x+16),int(sb.y+16),24,kText);
    DrawCircle(int(sb.x+28),int(sb.y+66),11,kTok[ci%4]);
    DrawText(cur.getUsername().c_str(),int(sb.x+50),int(sb.y+52),26,kText);
    DrawText(("M"+std::to_string(cur.getMoney())).c_str(),int(sb.x+16),int(sb.y+88),22,kAccentAlt);

    if(g->getBoard().size()>0 && cur.getPosition()<g->getBoard().size()){
        Tile* posTile=g->getBoard().getTileByIndex(cur.getPosition());
        if(posTile) DrawText((posTile->getCode()+" - "+posTile->getName()).c_str(),int(sb.x+16),int(sb.y+116),18,kSubtext);
    }

    // Progress bar
    Rectangle wave{sb.x+16,sb.y+150,sb.width-32,16};
    DrawRectangleRounded(wave,.8f,10,Fade(kPanelBorder,.35f));
    float prog = g->getTurnManager().getMaxTurn()>0?(float)g->getTurnManager().getCurrentTurn()/g->getTurnManager().getMaxTurn():.5f;
    DrawRectangleRounded({wave.x,wave.y,wave.width*prog,wave.height},.8f,10,kAccent);

    // Action buttons
    for(Button& b:actionButtons) b.draw();

    // Selected tile
    Tile* sel = (g->getBoard().size()>0 && selectedTile<g->getBoard().size()) ? g->getBoard().getTileByIndex(selectedTile) : nullptr;
    if(sel){
        Rectangle ins{sb.x,sb.y+404,sb.width,106};
        DrawRectangleRounded({ins.x+8,ins.y,ins.width-16,ins.height},.14f,8,Fade(kAccent,.1f));
        DrawText("Petak Dipilih",int(ins.x+16),int(ins.y+8),22,kText);
        DrawText((sel->getCode()+" - "+sel->getName()).c_str(),int(ins.x+16),int(ins.y+38),20,kText);
    }

    // All players
    DrawText("Semua Pemain",int(sb.x+16),int(sb.y+524),22,kText);
    for(size_t i=0;i<g->getPlayers().size();++i){
        float py=sb.y+556+i*28;
        DrawCircle(int(sb.x+20),int(py+8),6,kTok[i%4]);
        Player& p=g->getPlayers()[i];
        std::string row=p.getUsername()+" M"+std::to_string(p.getMoney());
        if(p.isBankrupt()) row+=" [BANGKRUT]";
        else if(p.getStatus()==PlayerStatus::JAILED) row+=" [PENJARA]";
        DrawText(row.c_str(),int(sb.x+36),int(py),17,int(i)==ci?kText:kSubtext);
    }

    // Hand cards
    DrawText("Kartu Kemampuan",int(sb.x+16),int(sb.y+684),22,kText);
    const auto& hand=cur.getHandCards();
    for(size_t i=0;i<hand.size();++i){
        float sway=sinf(sceneTime*1.7f+i)*3;
        Rectangle card{sb.x+16+i*100,sb.y+716+sway,88,38};
        DrawRectangleRounded(card,.22f,8,Fade(kAccent,.25f));
        DrawRectangleRoundedLinesEx(card,.22f,8,1.5f,Fade(kPanelBorder,.7f));
        DrawText(hand[i]->getName().c_str(),int(card.x+7),int(card.y+10),14,kText);
    }

    // Log
    DrawText("Log Terbaru",int(sb.x+16),int(sb.y+776),22,kText);
    auto entries=g->getLogger().getEntries();
    int lc=std::min<int>(3,int(entries.size()));
    for(int i=0;i<lc;++i){
        const LogEntry& e=entries[size_t(i)];
        float ly=sb.y+808+i*40;
        DrawText(("[T"+std::to_string(e.getTurn())+"] "+e.getUsername()+" | "+e.getActionType()).c_str(),int(sb.x+16),int(ly),16,kText);
        DrawText(e.getDetail().c_str(),int(sb.x+16),int(ly+17),15,kSubtext);
    }
}

void InGameScene::drawOverlay(Rectangle sr){
    if(overlayVis<=.01f||!overlayOpen)return;
    DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),Fade(kText,.38f*overlayVis));
    float rise=(1-overlayVis)*24;
    Rectangle p{sr.width*.5f-310,sr.height*.5f-252+rise,620,504};
    DrawRectangleRounded({p.x+5,p.y+9,p.width,p.height},.09f,10,Fade(kText,.12f*overlayVis));
    DrawRectangleRounded(p,.09f,10,Fade({250,255,235,255},overlayVis));
    DrawRectangleRoundedLinesEx(p,.09f,10,2.5f,Fade(kPanelBorder,overlayVis));
    drawSmallFlower(p.x+p.width-28,p.y+28,14,sceneTime*.5f,.5f*overlayVis);
    DrawText(overlayTitle.c_str(),int(p.x+22),int(p.y+20),32,kText);
    closeOverlayBtn.draw();
    float y=p.y+80;
    for(const auto& l:overlayLines){DrawText(l.c_str(),int(p.x+24),int(y),20,kSubtext);y+=32;if(y>p.y+p.height-40)break;}
}

void InGameScene::draw(){
    Rectangle sr{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},br{},sb{};
    layoutUi(sr,br,sb);
    drawBackground(sr); drawHeader(sr); drawBoard(br); drawSidebar(sb); drawOverlay(sr);
=======
    const std::vector<std::pair<std::string, std::function<void()>>> specs = {
        {"Lempar Dadu", [this]() { gameFacade->rollDice(); }},
        {"Beli", [this]() { gameFacade->buyCurrentProperty(); }},
        {"Gadai", [this]() { gameFacade->mortgageSelectedProperty(); }},
        {"Tebus", [this]() { gameFacade->redeemSelectedProperty(); }},
        {"Bangun", [this]() { gameFacade->buildSelectedProperty(); }},
        {"Info Petak", [this]() { gameFacade->openSelectedTileDetails(); }},
        {"Properti", [this]() { gameFacade->showCurrentPlayerProperties(); }},
        {"Kartu", [this]() { gameFacade->showCardPopup(); }},
        {"Festival", [this]() { gameFacade->showFestivalPopup(); }},
        {"Pajak", [this]() { gameFacade->showTaxPopup(); }},
        {"Lelang", [this]() { gameFacade->showAuctionPopup(); }},
        {"Simpan", [this]() { gameFacade->showSavePopup(); }},
        {"Akhir Giliran", [this]() { gameFacade->advanceTurn(); }},
        {"Kemenangan", [this]() { gameFacade->showVictoryPopup(); }},
    };

    for (const auto& spec : specs) {
        Button button(spec.first, kAccentAlt, {255,255,255,255});
        button.setOnClick(spec.second);
        actionButtons.push_back(button);
    }

    closeOverlayButton.setOnClick([this]() { gameFacade->closeOverlay(); });
    backToMenuButton.setOnClick([this]() { sceneManager->setScene(SceneType::MainMenu); });
}

void InGameScene::onEnter() {
    sceneTime = 0.0f;
    overlayVisibility = 0.0f;
    tokenPositions.clear();
    tokenPhases.clear();
}

void InGameScene::layoutUi(Rectangle screenRect, Rectangle& boardRect, Rectangle& sidebarRect) {
    const float sideMargin = 20.0f;
    const float topMargin = 88.0f;
    const float gap = 16.0f;
    const float sidebarW = std::max(330.0f, std::min(screenRect.width * 0.28f, 380.0f));
    const float boardSize = std::min(screenRect.height - topMargin - 24.0f,
                                     screenRect.width - sidebarW - gap - sideMargin * 2.0f);
    boardRect = {sideMargin, topMargin, boardSize, boardSize};
    sidebarRect = {boardRect.x + boardRect.width + gap, topMargin, sidebarW, boardSize};
}

Rectangle InGameScene::getTileRect(const Rectangle& boardRect, int index) const {
    const float cell = boardRect.width / 11.0f;
    if (index >= 0 && index <= 10) {
        return {boardRect.x + boardRect.width - (index + 1.0f) * cell,
                boardRect.y + boardRect.height - cell, cell, cell};
    }
    if (index >= 11 && index <= 19) {
        return {boardRect.x, boardRect.y + boardRect.height - (index - 10 + 1.0f) * cell, cell, cell};
    }
    if (index >= 20 && index <= 30) {
        return {boardRect.x + (index - 20.0f) * cell, boardRect.y, cell, cell};
    }
    return {boardRect.x + boardRect.width - cell, boardRect.y + (index - 30.0f) * cell, cell, cell};
}

Vector2 InGameScene::getTileCenter(const Rectangle& boardRect, int index) const {
    Rectangle r = getTileRect(boardRect, index);
    return {r.x + r.width * 0.5f, r.y + r.height * 0.5f};
}

void InGameScene::updateAnimations(const Rectangle& boardRect) {
    const GameViewModel& vm = gameFacade->getViewModel();
    const std::size_t playerCount = vm.players.size();
    if (tokenPositions.size() != playerCount) {
        tokenPositions.assign(playerCount, {0,0});
        tokenPhases.assign(playerCount, 0.0f);
        for (std::size_t i = 0; i < playerCount; ++i) {
            tokenPositions[i] = getTileCenter(boardRect, vm.players[i].position);
            tokenPhases[i] = static_cast<float>(i) * 1.37f;
        }
    }

    float dt = GetFrameTime();
    for (std::size_t i = 0; i < playerCount; ++i) {
        Vector2 target = getTileCenter(boardRect, vm.players[i].position);
        tokenPositions[i].x = ease(tokenPositions[i].x, target.x, dt * 6.0f);
        tokenPositions[i].y = ease(tokenPositions[i].y, target.y, dt * 6.0f);
        tokenPhases[i] += dt * (1.1f + i * 0.12f);
    }

    overlayVisibility = ease(overlayVisibility,
                             vm.overlay.type == OverlayType::None ? 0.0f : 1.0f,
                             dt * 8.0f);
}

void InGameScene::updateBoardSelection() {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
    Vector2 mouse = GetMousePosition();
    for (std::size_t i = 0; i < tileRects.size(); ++i) {
        if (CheckCollisionPointRec(mouse, tileRects[i])) {
            gameFacade->selectTile(static_cast<int>(i));
            return;
        }
    }
}

void InGameScene::update() {
    sceneTime += GetFrameTime();
    Rectangle screen{0,0,static_cast<float>(GetScreenWidth()),static_cast<float>(GetScreenHeight())};
    Rectangle boardRect{}, sidebarRect{};
    layoutUi(screen, boardRect, sidebarRect);

    const GameViewModel& vm = gameFacade->getViewModel();
    tileRects.clear();
    for (int i = 0; i < static_cast<int>(vm.board.size()); ++i) {
        tileRects.push_back(getTileRect(boardRect, i));
    }

    updateAnimations(boardRect);
    updateBoardSelection();

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (vm.overlay.type != OverlayType::None) gameFacade->closeOverlay();
        else sceneManager->setScene(SceneType::MainMenu);
    }

    backToMenuButton.setBoundary({screen.width - 130.0f, 22.0f, 106.0f, 42.0f});
    backToMenuButton.update();

    float buttonW = (sidebarRect.width - 14.0f) * 0.5f;
    float buttonH = 44.0f;
    float startY = sidebarRect.y + 238.0f;
    for (std::size_t i = 0; i < actionButtons.size(); ++i) {
        int row = static_cast<int>(i) / 2;
        int col = static_cast<int>(i) % 2;
        actionButtons[i].setBoundary({sidebarRect.x + col * (buttonW + 14.0f),
                                      startY + row * 54.0f,
                                      buttonW, buttonH});
        actionButtons[i].update();
    }

    if (overlayVisibility > 0.01f) {
        closeOverlayButton.setBoundary({screen.width * 0.5f + 268.0f, screen.height * 0.5f - 240.0f, 50.0f, 38.0f});
        closeOverlayButton.update();
    }
}

void InGameScene::drawBackground(Rectangle screenRect) {
    DrawRectangleGradientV(0,0,static_cast<int>(screenRect.width),static_cast<int>(screenRect.height),kBgA,kBgB);
    drawSmallFlower(40,40,18,sceneTime * 0.3f,0.3f);
    drawSmallFlower(screenRect.width - 50,50,14,sceneTime * 0.35f + 1,0.25f);
    drawSmallFlower(30,screenRect.height - 50,12,sceneTime * 0.4f + 2,0.25f);
    drawSmallFlower(screenRect.width - 40,screenRect.height - 40,16,sceneTime * 0.3f + 0.5f,0.25f);
}

void InGameScene::drawHeader(Rectangle) {
    const GameViewModel& vm = gameFacade->getViewModel();
    DrawText("Nimonspoli",22,16,34,kText);
    DrawText(vm.statusLine.c_str(),22,52,20,kSubtext);
    Rectangle badge{380,18,240,46};
    DrawRectangleRounded(badge,0.28f,8,Fade(kAccent,0.2f));
    DrawRectangleRoundedLinesEx(badge,0.28f,8,1.5f,Fade(kPanelBorder,0.8f));
    std::string turnText = "Turn " + std::to_string(vm.currentTurn) + " / " + std::to_string(vm.maxTurn);
    DrawText(turnText.c_str(),static_cast<int>(badge.x + 20),static_cast<int>(badge.y + 12),22,kText);
    backToMenuButton.draw();
}

void InGameScene::drawCenterPanel(const Rectangle& boardRect) {
    float cell = boardRect.width / 11.0f;
    Rectangle center{boardRect.x + cell, boardRect.y + cell, boardRect.width - 2.0f * cell, boardRect.height - 2.0f * cell};
    DrawRectangleRounded(center,0.05f,10,kCenterSurf);
    DrawRectangleRoundedLinesEx(center,0.05f,10,3,Fade(kAccentAlt,0.6f));
    DrawText("NIMONSPOLI",static_cast<int>(center.x + center.width * 0.27f),static_cast<int>(center.y + center.height * 0.43f),38,kText);
}

void InGameScene::drawBoard(const Rectangle& boardRect) {
    const GameViewModel& vm = gameFacade->getViewModel();
    DrawRectangleRounded({boardRect.x + 5, boardRect.y + 7, boardRect.width, boardRect.height},0.04f,10,Fade(kText,0.08f));
    DrawRectangleRounded(boardRect,0.04f,10,kBoardSurf);
    DrawRectangleRoundedLinesEx(boardRect,0.04f,10,3,Fade(kAccentAlt,0.7f));

    if (vm.board.empty()) {
        drawCenterPanel(boardRect);
        DrawText("Board belum tersedia", static_cast<int>(boardRect.x + 40), static_cast<int>(boardRect.y + 40), 24, kDanger);
        return;
    }

    for (std::size_t i = 0; i < vm.board.size() && i < tileRects.size(); ++i) {
        const TileViewData& tile = vm.board[i];
        Rectangle r = tileRects[i];
        bool selected = static_cast<int>(i) == vm.selectedTileIndex;
        DrawRectangleRec(r,kBoardSurf);
        DrawRectangleLinesEx(r,1,Fade(kPanelBorder,0.5f));

        Rectangle band = r;
        if (i <= 10) { band.height = 16; band.y = r.y + r.height - band.height; }
        else if (i <= 19) { band.width = 16; }
        else if (i <= 30) { band.height = 16; }
        else { band.width = 16; band.x = r.x + r.width - band.width; }
        DrawRectangleRec(band,groupColor(tile.colorGroup));

        if (selected) {
            DrawRectangleLinesEx(r,4,kAccent);
        }
        DrawText(tile.code.c_str(),static_cast<int>(r.x + 5),static_cast<int>(r.y + 5),16,kText);
        DrawText(tile.name.c_str(),static_cast<int>(r.x + 5),static_cast<int>(r.y + 25),10,kSubtext);

        if (tile.ownerIndex >= 0) {
            DrawCircle(static_cast<int>(r.x + r.width - 14),static_cast<int>(r.y + 14),7,kTokens[static_cast<std::size_t>(tile.ownerIndex) % kTokens.size()]);
        }
        if (tile.status == PropertyStatusView::Mortgaged) {
            DrawText("G",static_cast<int>(r.x + r.width - 22),static_cast<int>(r.y + 26),14,kDanger);
        }
        for (int h = 0; h < tile.houses && h < 4; ++h) {
            DrawRectangle(static_cast<int>(r.x + 7 + h * 11),static_cast<int>(r.y + r.height - 34),8,8,kAccentAlt);
        }
        if (tile.hotel) {
            DrawRectangle(static_cast<int>(r.x + 7),static_cast<int>(r.y + r.height - 34),16,10,kDanger);
        }
        if (tile.festivalMultiplier > 1) {
            std::string text = "x" + std::to_string(tile.festivalMultiplier);
            DrawText(text.c_str(),static_cast<int>(r.x + r.width - 26),static_cast<int>(r.y + r.height - 22),13,{180,50,200,255});
        }
    }

    drawCenterPanel(boardRect);

    for (std::size_t i = 0; i < tokenPositions.size(); ++i) {
        float bob = std::sin(tokenPhases[i] * 2.0f) * 4.0f;
        Vector2 pos{tokenPositions[i].x, tokenPositions[i].y + bob};
        Color tint = kTokens[i % kTokens.size()];
        DrawCircleLines(static_cast<int>(pos.x),static_cast<int>(pos.y),14 + std::sin(tokenPhases[i] * 2.5f) * 2.0f,Fade(tint,0.3f));
        DrawCircle(static_cast<int>(pos.x),static_cast<int>(pos.y),10,tint);
        DrawText(std::to_string(static_cast<int>(i) + 1).c_str(),static_cast<int>(pos.x - 4),static_cast<int>(pos.y - 6),12,kText);
    }
}

void InGameScene::drawSidebar(const Rectangle& sidebarRect) {
    const GameViewModel& vm = gameFacade->getViewModel();
    DrawRectangleRounded({sidebarRect.x + 4, sidebarRect.y + 7, sidebarRect.width, sidebarRect.height},0.04f,10,Fade(kText,0.07f));
    DrawRectangleRounded(sidebarRect,0.04f,10,Fade(kPanel,0.97f));
    DrawRectangleRoundedLinesEx(sidebarRect,0.04f,10,2,Fade(kPanelBorder,0.8f));

    DrawText("Giliran Sekarang",static_cast<int>(sidebarRect.x + 16),static_cast<int>(sidebarRect.y + 16),24,kText);
    if (!vm.players.empty() && vm.currentPlayerIndex < static_cast<int>(vm.players.size())) {
        const PlayerViewData& current = vm.players[static_cast<std::size_t>(vm.currentPlayerIndex)];
        DrawCircle(static_cast<int>(sidebarRect.x + 28),static_cast<int>(sidebarRect.y + 66),11,kTokens[static_cast<std::size_t>(vm.currentPlayerIndex) % kTokens.size()]);
        DrawText(current.name.c_str(),static_cast<int>(sidebarRect.x + 50),static_cast<int>(sidebarRect.y + 52),26,kText);
        DrawText(("M" + std::to_string(current.money)).c_str(),static_cast<int>(sidebarRect.x + 16),static_cast<int>(sidebarRect.y + 88),22,kAccentAlt);
    }

    if (!vm.board.empty() && vm.selectedTileIndex >= 0 && vm.selectedTileIndex < static_cast<int>(vm.board.size())) {
        const TileViewData& selected = vm.board[static_cast<std::size_t>(vm.selectedTileIndex)];
        DrawText((selected.code + " - " + selected.name).c_str(),static_cast<int>(sidebarRect.x + 16),static_cast<int>(sidebarRect.y + 116),18,kSubtext);
        DrawText(statusText(selected.status).c_str(),static_cast<int>(sidebarRect.x + 16),static_cast<int>(sidebarRect.y + 140),16,kSubtext);
    }

    for (Button& button : actionButtons) button.draw();

    DrawText("Semua Pemain",static_cast<int>(sidebarRect.x + 16),static_cast<int>(sidebarRect.y + 640),22,kText);
    for (std::size_t i = 0; i < vm.players.size(); ++i) {
        float py = sidebarRect.y + 672.0f + i * 28.0f;
        DrawCircle(static_cast<int>(sidebarRect.x + 20),static_cast<int>(py + 8),6,kTokens[i % kTokens.size()]);
        const PlayerViewData& p = vm.players[i];
        std::string row = p.name + " M" + std::to_string(p.money);
        if (p.status != "ACTIVE") row += " [" + p.status + "]";
        DrawText(row.c_str(),static_cast<int>(sidebarRect.x + 36),static_cast<int>(py),17,p.isCurrent ? kText : kSubtext);
    }

    DrawText("Log Terbaru",static_cast<int>(sidebarRect.x + 16),static_cast<int>(sidebarRect.y + 790),22,kText);
    int logCount = std::min<int>(4, static_cast<int>(vm.logs.size()));
    for (int i = 0; i < logCount; ++i) {
        const LogEntryViewData& entry = vm.logs[static_cast<std::size_t>(i)];
        float y = sidebarRect.y + 822.0f + i * 40.0f;
        DrawText(("[T" + std::to_string(entry.turn) + "] " + entry.actor + " | " + entry.type).c_str(),
                 static_cast<int>(sidebarRect.x + 16),static_cast<int>(y),16,kText);
        DrawText(entry.detail.c_str(),static_cast<int>(sidebarRect.x + 16),static_cast<int>(y + 17),15,kSubtext);
    }
}

void InGameScene::drawOverlay(Rectangle screenRect) {
    const GameViewModel& vm = gameFacade->getViewModel();
    if (overlayVisibility <= 0.01f || vm.overlay.type == OverlayType::None) return;

    DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),Fade(kText,0.38f * overlayVisibility));
    Rectangle panel{screenRect.width * 0.5f - 310.0f,
                    screenRect.height * 0.5f - 252.0f + (1.0f - overlayVisibility) * 24.0f,
                    620.0f,504.0f};
    DrawRectangleRounded({panel.x + 5, panel.y + 9, panel.width, panel.height},0.09f,10,Fade(kText,0.12f * overlayVisibility));
    DrawRectangleRounded(panel,0.09f,10,Fade({250,255,235,255},overlayVisibility));
    DrawRectangleRoundedLinesEx(panel,0.09f,10,2.5f,Fade(kPanelBorder,overlayVisibility));
    DrawText(vm.overlay.title.c_str(),static_cast<int>(panel.x + 22),static_cast<int>(panel.y + 20),32,kText);
    closeOverlayButton.draw();

    float y = panel.y + 80.0f;
    for (const std::string& line : vm.overlay.lines) {
        DrawText(line.c_str(),static_cast<int>(panel.x + 24),static_cast<int>(y),20,kSubtext);
        y += 32.0f;
        if (y > panel.y + panel.height - 70.0f) break;
    }
    if (!vm.overlay.footer.empty()) {
        DrawText(vm.overlay.footer.c_str(),static_cast<int>(panel.x + 24),static_cast<int>(panel.y + panel.height - 42),18,kSubtext);
    }
}

void InGameScene::draw() {
    Rectangle screen{0,0,static_cast<float>(GetScreenWidth()),static_cast<float>(GetScreenHeight())};
    Rectangle boardRect{}, sidebarRect{};
    layoutUi(screen, boardRect, sidebarRect);
    drawBackground(screen);
    drawHeader(screen);
    drawBoard(boardRect);
    drawSidebar(sidebarRect);
    drawOverlay(screen);
>>>>>>> origin
}
