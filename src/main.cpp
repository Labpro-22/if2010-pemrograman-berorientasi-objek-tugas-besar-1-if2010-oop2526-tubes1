#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "../include/core/GameContext.hpp"
#include "../include/core/GameLoop.hpp"
#include "../include/core/MovementHandler.hpp"
#include "../include/core/SkillCardManager.hpp"
#include "../include/core/TurnManager.hpp"
#include "../include/models/Player.hpp"
#include "../include/utils/Dice.hpp"

namespace {
std::string statusLabel(PlayerStatus status) {
	if (status == ACTIVE) return "ACTIVE";
	if (status == BANKRUPT) return "BANKRUPT";
	return "JAILED";
}

void printState(const std::vector<Player*>& players, const std::string& title) {
	std::cout << "\n=== " << title << " ===" << std::endl;
	for (Player* player : players) {
		std::cout << player->getUsername()
				  << " | money=" << player->getMoney()
				  << " | pos=" << player->getPosition()
				  << " | status=" << statusLabel(player->getStatus())
				  << " | cards=" << player->getHand().size() << std::endl;
	}
}
}  

int main() {
	// Seed tetap biar test konsisten tiap run.
	std::srand(42);

	Player* p1 = new Player("A", 1500);
	Player* p2 = new Player("B", 1500);
	Player* p3 = new Player("C", 1500);

	std::vector<Player*> players = {p1, p2, p3};

	TurnManager turnManager(players, 20);
	MovementHandler movementHandler(nullptr, 40, 200);
	SkillCardManager skillCardManager(3);
	Dice dice;

	GameContext ctx;
	ctx.board = nullptr;
	ctx.turnManager = &turnManager;
	ctx.movementHandler = &movementHandler;
	ctx.skillCardManager = &skillCardManager;
	ctx.allPlayers = players;

	printState(players, "Initial State");

	// Skenario 1: lewat GO biar dapat gaji.
	p1->setPosition(39);
	movementHandler.movePlayer(p1, 4);
	printState(players, "After Pass GO Test");

	// Skenario 2:  alur penjara.
	movementHandler.sendToJail(p2);
	printState(players, "After Send To Jail Test");

	// Skenario 3: bagikan kartu ke semua pemain lalu coba pakai satu sebelum lempar dadu.
	skillCardManager.distributeCardToAll(players);
	if (!p1->getHand().empty()) {
		skillCardManager.useCard(p1, 0, &ctx);
	}
	printState(players, "After Card Distribution/Use");

	// Skenario 4: jalankan turn GameLoop.
	for (int turn = 1; turn <= 6; ++turn) {
		GameLoop::playTurn(&ctx, dice);
		printState(players, "After GameLoop Turn " + std::to_string(turn));
	}

	std::cout << "\n test selesai." << std::endl;

	delete p1;
	delete p2;
	delete p3;

	return 0;
}
