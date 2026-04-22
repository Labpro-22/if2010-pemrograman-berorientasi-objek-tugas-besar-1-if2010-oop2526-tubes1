#include "../../include/core/MovementHandler.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/utils/Dice.hpp"

MovementHandler::MovementHandler(GameBoard* b, int boardSizeValue, int goSalaryValue, int jailFineValue) {
    board = b;
    // Placeholder buat tes sebelum ukuran board dibaca dari config/board object.
    boardSize = (boardSizeValue > 0) ? boardSizeValue : 40;
    // Placeholder buat tes sebelum salary GO dibaca dari special config.
    goSalary = (goSalaryValue >= 0) ? goSalaryValue : 200;
    jailFine = (jailFineValue >= 0) ? jailFineValue : 50;
}

void MovementHandler::movePlayer(Player* player, int steps) {
    if (player == nullptr) {
        return;
    }
    if (!player) return;
    if (player->getStatus() == BANKRUPT) return;
    if (player->getStatus() == JAILED) return; // handle via handleJailTurn

    int oldPos = player->getPosition();

    int rawNewPos = oldPos + steps;
    int newPos = rawNewPos % boardSize;
    if (newPos < 0) {
        newPos += boardSize;
    }

    if (rawNewPos >= boardSize) {
        handlePassGo(player);
    }

    player->setPosition(newPos);

    // TODO: tunggu implementasi GameBoard/Tile 
    //  aktifin pemanggilan tile->onLand(player) di area tujuan.
    // Tile* tile = board->getTileAt(newPos);
    // tile->onLand(player);
}

void MovementHandler::teleportPlayer(Player* player, int target) {
    if (player == nullptr) {
        return;
    }

    int normalizedTarget = target % boardSize;
    if (normalizedTarget < 0) {
        normalizedTarget += boardSize;
    }

    player->setPosition(normalizedTarget);

    // TODO: tunggu implementasi GameBoard/Tile,
    // sambung teleport ke board->getTileAt(normalizedTarget)->onLand(player).
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
    if (player == nullptr) {
        return;
    }

    // TODO: tunggu parser/config jadi,to goSalary dari special config (GO salary).
    (*player) += goSalary;
}

void MovementHandler::sendToJail(Player* player) {
    if (player == nullptr) {
        return;
    }

    player->setStatus(JAILED);
    player->resetConsecutiveDoubles();
    player->resetJailTurns();

    // TODO: tunggu implementasi GameBoard/Tile,
    // ganti posisi placeholder ini dengan board->getJailIndex().
    player->setPosition(10);
}

JailResult MovementHandler::handleJailTurn(Player* player, Dice& dice) {
    if (!player) return STILL_JAILED;

    // Jika sudah 3 kali gagal → giliran ini wajib bayar denda, tidak roll
    if (player->getJailTurnsRemaining() >= 3) {
        player->setStatus(ACTIVE);
        player->resetJailTurns();
        (*player) -= jailFine; 

        return FORCED_OUT;
    }

    // Coba roll
    dice.rollRandom();
    if (dice.isDouble()) {
        player->setStatus(ACTIVE);
        player->resetJailTurns();
        return ESCAPED_DOUBLE;
    }

    player->incrementJailTurns();
    return STILL_JAILED;
}