#include "core/tiles.h"
#include "core/game.h"
#include "core/player.h"
#include "core/propertytypes.h"
#include <iostream>

namespace Nimonspoli {


void PropertyTile::onLanded(Player& player, Game& game) {
    if (property_->isBank()) {
        game.handlePropertyPurchase(player, *property_);
    } else if (property_->isOwned()) {
        game.handleRentPayment(player, *property_, game.lastDiceTotal());
    }
    // mortgage ga ngapa2in
}

void GoTile::onLanded(Player& player, Game& game) {
    // Salary already dah dapat di movePlayer().
    (void)player; (void)game; //Biar ga kena squiggly lines aj
}

void JailTile::onLanded(Player& player, Game& game) {
    (void)jailFine_; (void)game;
    game.logger().log(game.currentTurn(), player.username(),
                      "MAMPIR", "Hanya mampir di Penjara");
}

void GoToJailTile::onLanded(Player& player, Game& game) {
    game.sendToJail(player);
}

void TaxTile::onLanded(Player& player, Game& game) {
    if (taxType_ == TaxType::PPH) {
        game.handleTaxPPH(player);
    } else {
        game.handleTaxPBM(player);
    }
}

void FestivalTile::onLanded(Player& player, Game& game) {
    game.handleFestival(player);
}

void ChanceTile::onLanded(Player& player, Game& game) {
    ChanceCard* card = game.chanceDeck().draw();
    game.logger().log(game.currentTurn(), player.username(),
                      "KESEMPATAN", card->description());
    card->execute(player, game);
    game.chanceDeck().discard(card);
}

void CommunityTile::onLanded(Player& player, Game& game) {
    CommunityCard* card = game.communityDeck().draw();
    game.logger().log(game.currentTurn(), player.username(),
                      "DANA_UMUM", card->description());
    card->execute(player, game);
    game.communityDeck().discard(card);
}

} 