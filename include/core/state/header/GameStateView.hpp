#ifndef CORE_STATE_GAME_STATE_VIEW_HPP
#define CORE_STATE_GAME_STATE_VIEW_HPP

#include <string>

#include "core/state/header/GameState.hpp"

class GameStateView {
public:
	std::string render(const GameState& state) const;

private:

protected:
};

#endif
