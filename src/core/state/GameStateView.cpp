#include "core/state/header/GameStateView.hpp"

std::string GameStateView::render(const GameState& state) const {
	std::string phase = "SETUP";
	if (state.getPhase() == GamePhase::RUNNING) {
		phase = "RUNNING";
	} else if (state.getPhase() == GamePhase::FINISHED) {
		phase = "FINISHED";
	}
	return "Phase: " + phase + " | Turn: " + std::to_string(state.getTurn());
}
