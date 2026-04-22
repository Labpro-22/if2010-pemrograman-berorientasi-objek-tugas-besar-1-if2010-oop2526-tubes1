#include "../../include/core/MovementHandler.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/utils/Dice.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/models/GameBoard.hpp"
#include "../../include/models/JailTile.hpp"

MovementHandler::MovementHandler(GameBoard* b, int boardSizeValue, int goSalaryValue, int jailFineValue) {
    board = b;
    // Placeholder buat tes sebelum ukuran board dibaca dari config/board object.
    boardSize = (boardSizeValue > 0) ? boardSizeValue : 40;
    // Placeholder buat tes sebelum salary GO dibaca dari special config.
    goSalary = (goSalaryValue >= 0) ? goSalaryValue : 200;
    jailFine = (jailFineValue >= 0) ? jailFineValue : 50;
}

void MovementHandler::movePlayer(Player* player, int steps) {
    if (!player) return;
    if (player->getStatus() == BANKRUPT) return;
    if (player->getStatus() == JAILED) return;

    int oldPos = player->getPosition();
    int rawNewPos = oldPos + steps;
    int newPos = rawNewPos % boardSize;
    if (newPos < 0) newPos += boardSize;

    // Handle pass GO
    if (rawNewPos >= boardSize) {
        handlePassGo(player);
        
    }

    player->setPosition(newPos);

    if (board != nullptr) {
        Tile* tile = board->getTileAt(newPos);
        if (tile != nullptr) {
            tile->onLand(player);
        }
    }
}

void MovementHandler::teleportPlayer(Player* player, int target) {
    if (!player) return;

    int normalizedTarget = target % boardSize;
    if (normalizedTarget < 0) normalizedTarget += boardSize;

    player->setPosition(normalizedTarget);

    if (board != nullptr) {
        Tile* tile = board->getTileAt(normalizedTarget);
        if (tile != nullptr) {
            tile->onLand(player);
        }
    }
}

void MovementHandler::pullPlayer(Player* target, int toPosition) {
    if (target == nullptr) {
        return;
    }

    // TODO: tunggu implementasi board/tile, cek apakah pull perlu trigger onLand().
    int normalizedTarget = toPosition % boardSize;
    if (normalizedTarget < 0) {
        normalizedTarget += boardSize;
    }
    target->setPosition(normalizedTarget);
}

void MovementHandler::handlePassGo(Player* player) {
    if (!player) return;

    // Cari GoTile di board 
    if (board != nullptr) {
        Tile* goTile = board->getTileAt(0);
        if (goTile != nullptr) {
            goTile->onPass(player); 
            return;
        }
    }

    // Fallback 
    (*player) += goSalary;
}

void MovementHandler::sendToJail(Player* player) {
    if (!player) return;

    player->setStatus(JAILED);
    player->resetConsecutiveDoubles();
    player->resetJailTurns();

    // Cari posisi jail dari board
    if (board != nullptr) {
        for (int i = 0; i < boardSize; i++) {
            Tile* tile = board->getTileAt(i);
            // Dynamic cast untuk cek apakah JailTile
            JailTile* jailTile = dynamic_cast<JailTile*>(tile);
            if (jailTile != nullptr) {
                player->setPosition(i);
                // Tidak trigger onLand — sendToJail bukan landing normal
                return;
            }
        }
    }

    // Fallback
    player->setPosition(10);
}

JailResult MovementHandler::handleJailTurn(Player* player, Dice& dice) {
    if (!player) return STILL_JAILED;

    // Cari JailTile
    JailTile* jailTile = nullptr;
    if (board != nullptr) {
        for (int i = 0; i < boardSize; i++) {
            jailTile = dynamic_cast<JailTile*>(board->getTileAt(i));
            if (jailTile != nullptr) break;
        }
    }

    int fine = (jailTile != nullptr) ? jailTile->getFineAmount() : jailFine;

    // Sudah 3 kali gagal → giliran ini wajib bayar denda
    if (player->getJailTurnsRemaining() >= 3) {
        player->setStatus(ACTIVE);
        player->resetJailTurns();
        (*player) -= fine;
        return FORCED_OUT;
    }

    // Coba roll
    if (dice.isDouble()) {
        player->setStatus(ACTIVE);
        player->resetJailTurns();
        return ESCAPED_DOUBLE;
    }

    player->incrementJailTurns();
    return STILL_JAILED;
}