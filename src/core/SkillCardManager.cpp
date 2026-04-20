#include "../../include/core/SkillCardManager.hpp"
#include "../../include/utils/SkillCard.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/core/GameContext.hpp"

#include <cstdlib>

SkillCardManager::SkillCardManager(int maxSize) {
    maxHandSize = (maxSize > 0) ? static_cast<size_t>(maxSize) : static_cast<size_t>(0);
}

bool SkillCardManager::isValidIndex(Player* player, int idx) {
    if (player == nullptr) {
        return false;
    }

    return idx >= 0 && idx < static_cast<int>(player->getHand().size());
}

void SkillCardManager::distributeCardToAll(vector<Player*> players) {
    for (Player* player : players) {
        distributeCardTo(player);
    }
}

void SkillCardManager::distributeCardTo(Player* player) {
    if (player == nullptr) {
        return;
    }

    if (player->getHand().size() >= maxHandSize) return;

    SkillCard* card = generateCard();
    player->receiveCard(card);
}

void SkillCardManager::useCard(Player* player, int idx, GameContext* ctx) {
    if (player == nullptr || ctx == nullptr) {
        return;
    }

    if (player->hasUsedCard() || player->hasRolled()) return;

    if (!isValidIndex(player, idx)) {
        return;
    }

    SkillCard* card = player->removeCard(idx);
    if (card == nullptr) {
        return;
    }

    card->activate(player, ctx);

    player->markCardUsed();

    delete card;
}

void SkillCardManager::dropCard(Player* player, int idx) {
    if (!isValidIndex(player, idx)) return;

    SkillCard* card = player->removeCard(idx);
    delete card;
}

void SkillCardManager::decrementDurations(Player* player) {
    if (player == nullptr) {
        return;
    }

    for (auto c : player->getHand()) {
        if (c != nullptr) {
            c->decrementDuration();
        }
    }
}

SkillCard* SkillCardManager::generateCard() {
    int r = rand() % 3;

    if (r == 0) return new MoveCard(rand() % 6 + 1);
    if (r == 1) return new ShieldCard();
    // Placeholder: 40 ngikut default board size yg placeholder juga, nanti gunakan ukuran board asli dari config
    return new TeleportCard(rand() % 40);
}
