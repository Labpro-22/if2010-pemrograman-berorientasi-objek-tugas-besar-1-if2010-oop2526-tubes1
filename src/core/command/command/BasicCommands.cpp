#include "core/command/header/command/BasicCommands.hpp"

#include "core/engine/header/GameEngine.hpp"
#include "core/state/header/GameStateView.hpp"
#include "models/Player.hpp"

RollDiceCommand::RollDiceCommand() : Command("roll_dice", "Roll dice for current turn") {}

void RollDiceCommand::execute(GameEngine& engine, Player& player) {
	engine.processCommand("lempar", player);
}

bool RollDiceCommand::canExecute(const GameStateView& state) const {
	return !state.getHasRolledDice();
}

EndTurnCommand::EndTurnCommand() : Command("end_turn", "End current player's turn") {}

void EndTurnCommand::execute(GameEngine& engine, Player& player) {
	engine.processCommand("selesai", player);
}

bool EndTurnCommand::canExecute(const GameStateView& state) const {
	(void)state;
	return true;
}

PrintBoardCommand::PrintBoardCommand() : Command("print_board", "Display board snapshot") {}

void PrintBoardCommand::execute(GameEngine& engine, Player& player) {
	engine.processCommand("cetak papan", player);
}

PrintLogCommand::PrintLogCommand() : Command("print_log", "Display transaction log") {}

void PrintLogCommand::execute(GameEngine& engine, Player& player) {
	engine.processCommand("cetak log", player);
}
