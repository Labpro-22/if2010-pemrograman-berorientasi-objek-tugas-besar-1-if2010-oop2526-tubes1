#include "core/COMPlayer.hpp"

#include "core/Property.hpp"
#include "logic/Game.hpp"

#include <random>
#include <utility>

namespace core {

namespace {

int rollDie(std::mt19937& gen) {
    std::uniform_int_distribution<int> dist(1, 6);
    return dist(gen);
}

}  // namespace

COMPlayer::COMPlayer(std::string name, std::string token, std::unique_ptr<COMStrategy> strategy)
        :   Player(std::move(name), std::move(token)),
            strategy_(std::move(strategy)) {}

void COMPlayer::setStrategy(std::unique_ptr<COMStrategy> strategy) {
    strategy_ = std::move(strategy);
}

COMStrategy* COMPlayer::getStrategy() const noexcept {
    return strategy_.get();
}

int COMPlayer::promptChoice(const std::string& context, int defaultIndex, int optionCount) {
    if (strategy_ != nullptr) {
        return strategy_->suggestChoice(context, defaultIndex, optionCount);
    }
    return Player::promptChoice(context, defaultIndex, optionCount);
}

void COMPlayer::takeTurn(logic::Game& game) {
    resetPerTurnFlags();

    std::random_device rd;
    std::mt19937 gen(rd());
    const int die1 = rollDie(gen);
    const int die2 = rollDie(gen);
    game.setLastDiceRoll(die1, die2);
    const int total = die1 + die2;
    game.moveCurrentPlayer(total);
    game.logEvent("COM_ROLL", *this, total);

    if (strategy_ != nullptr) {
        for (Property* lot : getOwnedProperties()) {
            if (lot == nullptr) {
                continue;
            }
            if (strategy_->shouldMortgage(lot, *this, game)) {
                game.mortgageProperty(lot);
                game.logEvent("COM_MORTGAGE", *this, *lot, 0);
            }
        }

        for (Property* lot : getOwnedProperties()) {
            if (lot == nullptr) {
                continue;
            }
            if (strategy_->shouldBuild(lot, *this, game)) {
                game.buildHouse(this, nullptr);
                game.logEvent("COM_BUILD", *this, *lot, 0);
                break;
            }
        }

        for (Property* lot : getOwnedProperties()) {
            if (lot == nullptr) {
                continue;
            }
            if (strategy_->shouldBuy(lot, *this, game)) {
                game.buyProperty(lot);
                game.logEvent("COM_BUY", *this, *lot, 0);
                break;
            }
        }
    }
}

bool COMPlayer::isHuman() { return false; }

}
