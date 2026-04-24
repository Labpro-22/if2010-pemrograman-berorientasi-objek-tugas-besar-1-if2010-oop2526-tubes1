#include "GameScreen.hpp"
#include "GameScreenTiles.hpp"
#include "../../core/GameMaster/GameMaster.hpp"
#include "../../core/Board/Board.hpp"
#include "../../core/Property/Property.hpp"
#include "../../core/Property/RailroadProperty.hpp"
#include "../../core/Property/UtilityProperty.hpp"
#include "../../core/Player/Player.hpp"

// ─────────────────────────────────────────────────────────────────────────────
//  syncFromGameMaster()
// ─────────────────────────────────────────────────────────────────────────────
void GameScreen::syncFromGameMaster()
{
    if (!isRealMode())
        return;

    GameMaster*       gm    = guiManager->getGameMaster();
    const GameState&  gs    = gm->getState();
    Board*            board = gs.getBoard();

    gameState.currentTurn     = gs.getCurrTurn();
    gameState.maxTurn         = gs.getMaxTurn();
    gameState.activePlayerIdx = gs.getCurrPlayerIdx();

    // ── Sync Players ──────────────────────────────────────────────────────
    const auto& realPlayers = gs.getPlayers();
    if (playerVisuals.size() != realPlayers.size()) {
        playerVisuals.resize(realPlayers.size());
        for (int i = 0; i < (int)realPlayers.size(); i++) {
            playerVisuals[i].currentTileIdx = (float)(realPlayers[i]->getPosition());
            playerVisuals[i].targetTileIdx  = playerVisuals[i].currentTileIdx;
        }
    }

    gameState.players.clear();
    gameState.players.resize(realPlayers.size());

    for (int i = 0; i < (int)realPlayers.size(); ++i) {
        Player*      p  = realPlayers[i];
        MockPlayer&  mp = gameState.players[i];

        mp.username  = p->getUsername();
        mp.money     = p->getBalance();
        int realPos  = p->getPosition();
        mp.position  = realPos;
        mp.cardCount = p->getHandSize();
        mp.status    = (p->getStatus() == PlayerStatus::JAILED)   ? "JAILED"
                     : (p->getStatus() == PlayerStatus::BANKRUPT) ? "BANKRUPT"
                                                                   : "ACTIVE";

        if (realPos != (int)playerVisuals[i].targetTileIdx)
            playerVisuals[i].targetTileIdx = (float)realPos;
    }

    // ── Sync Properties ───────────────────────────────────────────────────
    if (board) {
        for (int i = 0; i < (int)gameState.properties.size(); ++i) {
            Tile* tile = board->getTile(i);
            if (!tile) continue;

            PropertyTile* pt = dynamic_cast<PropertyTile*>(tile);
            if (!pt) continue;

            Property* prop = pt->getProperty();
            if (!prop) continue;

            MockProperty& mp = gameState.properties[i];

            mp.name       = prop->getName();
            mp.colorGroup = prop->getColorGroup();

            const std::string& ownerId = prop->getOwnerId();
            mp.owner     = -1;
            mp.mortgaged = (prop->getStatus() == PropertyStatus::MORTGAGED);

            if (prop->getStatus() != PropertyStatus::BANK) {
                for (int pi = 0; pi < (int)realPlayers.size(); ++pi) {
                    if (realPlayers[pi]->getUsername() == ownerId) {
                        mp.owner = pi;
                        break;
                    }
                }
            }
            mp.price = prop->getPurchasePrice();

            if (auto* sp = dynamic_cast<StreetProperty*>(prop)) {
                mp.type      = "STREET";
                mp.buildings = sp->gethasHotel() ? 5 : sp->getBuildingCount();
                mp.festivalMult = sp->getFestivalMultiplier();
                mp.festivalDur  = sp->getFestivalDuration();
            } else {
                if      (dynamic_cast<RailroadProperty*>(prop)) mp.type = "RAILROAD";
                else if (dynamic_cast<UtilityProperty*>(prop))  mp.type = "UTILITY";
                else                                             mp.type = "ACTION";

                mp.buildings    = 0;
                mp.festivalMult = 1;
                mp.festivalDur  = 0;
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  syncDiceResult()
// ─────────────────────────────────────────────────────────────────────────────
void GameScreen::syncDiceResult()
{
    if (!isRealMode())
        return;

    GameMaster* gm   = guiManager->getGameMaster();
    Dice*       dice = gm->getState().getDice();
    if (!dice) return;

    int v1 = dice->getDaduVal1();
    int v2 = dice->getDaduVal2();

    bool justRolled = gm->getState().getHasRolled() && !diceState.hasRolled;
    bool valChanged = (v1 != diceState.val1 || v2 != diceState.val2);

    if ((valChanged || justRolled) && v1 > 0) {
        diceState.val1      = v1;
        diceState.val2      = v2;
        diceState.animating = true;
        diceState.animTimer = 0.f;
    }
    diceState.hasRolled = gm->getState().getHasRolled();

    // ── Trigger Dialogs ────────────────────────────────────────────────────
    const GameState& gs = gm->getState();
    if (gs.getPhase() == GamePhase::AWAITING_BUY && !buyDialog.visible)
        triggerBuyDialog(gs.getCurrPlayer()->getPosition());
    if (gs.getPhase() == GamePhase::AWAITING_TAX && !taxDialog.visible)
        triggerTaxDialog();
    if (gs.getPhase() == GamePhase::AWAITING_FESTIVAL && !festivalDialog.visible)
        triggerFestivalDialog();
    if (gs.getPhase() == GamePhase::SHOW_CARD && !cardDialog.visible)
        triggerCardDialog();
    if (gs.getPhase() == GamePhase::AUCTION && !auctionDialog.visible)
        triggerAuctionDialog();
    if (gs.getPhase() == GamePhase::AWAITING_JAIL && !jailDialog.visible)
        triggerJailDialog();
    // Fase BANKRUPTCY → tampilkan dialog likuidasi
    if (gs.getPhase() == GamePhase::BANKRUPTCY
        && !bankruptcyDialog.visible
        && !bankruptcyDialog.notifVisible)
    {
        triggerBankruptcyDialog();
    }

    // Setelah lelang selesai (properti bangkrut ke Bank), mulai lelang berikutnya
    // jika masih ada antrian
    if (gs.getPhase() == GamePhase::PLAYER_TURN
        && !gs.getPendingAuctionQueue().empty()
        && !auctionDialog.visible)
    {
        Property* next = gm->getState().popPendingAuction();
        if (next)
            gm->startAuction(next, nullptr);
    }
}