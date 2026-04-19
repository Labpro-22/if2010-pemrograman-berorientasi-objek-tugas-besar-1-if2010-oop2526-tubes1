#include "../../include/models/CommunityCards.hpp"

#include "../../include/core/GameEngine.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/utils/GameException.hpp"

BirthdayCard::BirthdayCard(int amountPerPlayer)
    : ActionCard("Ini adalah hari ulang tahun Anda. Dapatkan M100 dari setiap pemain."),
      amountPerPlayer(amountPerPlayer) {}

void BirthdayCard::apply(Player& player, GameEngine& game) {
    std::vector<Player*> activePlayers = game.getActivePlayers();

    for (Player* other : activePlayers) {
        if (!other || other == &player || other->isBankrupt()) {
            continue;
        }

        if (other->isShieldActive()) {
            continue;
        }

        if (!other->canAfford(amountPerPlayer)) {
            throw InsufficientFundsException(other->getUsername(), amountPerPlayer, other->getMoney());
        }

        other->deductMoney(amountPerPlayer);
        player.addMoney(amountPerPlayer);
    }
}

DoctorFeeCard::DoctorFeeCard(int fee)
    : ActionCard("Biaya dokter. Bayar M700."), fee(fee) {}

void DoctorFeeCard::apply(Player& player, GameEngine& game) {
    (void)game;

    if (player.isShieldActive()) {
        return;
    }

    if (!player.canAfford(fee)) {
        throw InsufficientFundsException(player.getUsername(), fee, player.getMoney());
    }

    player.deductMoney(fee);
}

ElectionCard::ElectionCard(int amountPerPlayer)
    : ActionCard("Anda mau nyaleg. Bayar M200 kepada setiap pemain."),
      amountPerPlayer(amountPerPlayer) {}

void ElectionCard::apply(Player& player, GameEngine& game) {
    if (player.isShieldActive()) {
        return;
    }

    std::vector<Player*> activePlayers = game.getActivePlayers();
    for (Player* other : activePlayers) {
        if (!other || other == &player || other->isBankrupt()) {
            continue;
        }

        if (!player.canAfford(amountPerPlayer)) {
            throw InsufficientFundsException(player.getUsername(), amountPerPlayer, player.getMoney());
        }

        player.deductMoney(amountPerPlayer);
        other->addMoney(amountPerPlayer);
    }
}
