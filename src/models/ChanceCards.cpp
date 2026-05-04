#include "../../include/models/ChanceCards.hpp"

#include "../../include/core/GameEngine.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Property.hpp"
#include "../../include/models/PropertyTile.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/utils/GameException.hpp"

GoToNearestRailroadCard::GoToNearestRailroadCard()
    : ActionCard("Pergi ke stasiun terdekat.") {}

void GoToNearestRailroadCard::apply(Player& player, GameEngine& game) {
    Board& board = game.getBoard();

    const int current = player.getPosition();
    int bestDistance = board.size() + 1;
    int bestIndex = -1;

    for (PropertyTile* tile : board.getAllPropertyTiles()) {
        if (!tile) {
            continue;
        }

        const Property& property = tile->getProperty();
        if (property.getType() != PropertyType::RAILROAD) {
            continue;
        }

        const int idx = tile->getIndex();
        const int dist = board.distanceTo(current, idx);

        if (dist > 0 && dist < bestDistance) {
            bestDistance = dist;
            bestIndex = idx;
        }
    }

    if (bestIndex < 0) {
        throw GameException("No railroad tile found for GoToNearestRailroadCard.");
    }

    if (bestIndex < current) {
        player.addMoney(game.getGoSalary());
    }

    player.setPosition(bestIndex);
    Tile& landing = board.getTileByIndex(bestIndex);
    game.handleLanding(player, landing);
}

MoveBackCard::MoveBackCard(int steps)
    : ActionCard("Mundur 3 petak."), steps(steps) {}

void MoveBackCard::apply(Player& player, GameEngine& game) {
    Board& board = game.getBoard();
    player.move(-steps, board.size());

    Tile& landing = board.getTileByIndex(player.getPosition());
    game.handleLanding(player, landing);
}

GoToJailCard::GoToJailCard()
    : ActionCard("Masuk Penjara.") {}

void GoToJailCard::apply(Player& player, GameEngine& game) {
    Board& board = game.getBoard();
    const int jailIndex = board.getIndexOf("PEN");

    player.setPosition(jailIndex);
    player.setStatus(PlayerStatus::JAILED);
    player.setJailTurns(0);
}

GetOutOfJailCard::GetOutOfJailCard()
    : ActionCard("Bebas dari Penjara.") {}

void GetOutOfJailCard::apply(Player& player, GameEngine& game) {
    (void)game;
    player.addJailFreeCard();
}
