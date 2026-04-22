#include "../../include/core/SkillCardManager.hpp"
#include "../../include/utils/SkillCard.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/core/GameContext.hpp"

#include <cstdlib>

SkillCardManager::SkillCardManager(int maxSize) {
    maxHandSize = (maxSize > 0) ? static_cast<size_t>(maxSize) : static_cast<size_t>(0);
}

void SkillCardManager::initDeck() {
    // Bersihkan deck lama jika ada
    for (SkillCard* c : deck) delete c;
    deck.clear();
    discardPile.clear();

    // Stok sesuai spec: Move=4, Discount=3, Shield=2, Teleport=2, Lasso=2, Demolition=2
    for (int i = 0; i < 4; i++) deck.push_back(new MoveCard(rand() % 6 + 1));
    for (int i = 0; i < 3; i++) deck.push_back(new DiscountCard(rand() % 30 + 10));
    for (int i = 0; i < 2; i++) deck.push_back(new ShieldCard());
    for (int i = 0; i < 2; i++) deck.push_back(new TeleportCard(rand() % 40));
    for (int i = 0; i < 2; i++) deck.push_back(new LassoCard());
    for (int i = 0; i < 2; i++) deck.push_back(new DemolitionCard());

    // Shuffle
    for (int i = (int)deck.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(deck[i], deck[j]);
    }
}

void SkillCardManager::reshuffleIfEmpty() {
    if (!deck.empty()) return;
    deck = discardPile;
    discardPile.clear();

    for (int i = (int)deck.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(deck[i], deck[j]);
    }
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

SkillCard* SkillCardManager::distributeCardTo(Player* player) {
    if (player == nullptr) return nullptr;

    SkillCard* card = generateCard();

    if (player->getHand().size() >= maxHandSize) {
        // Kasih kartu ke player dulu 4
        // return kartu agar UI  bisa trigger drop prompt
        player->receiveCard(card);
        return card; 
    }

    player->receiveCard(card);
    return nullptr; 
}

void SkillCardManager::useCard(Player* player, int idx, GameContext* ctx) {
    
    if (player == nullptr || ctx == nullptr) {
        return;
    }
    if (player->getStatus() == JAILED) return;
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
    discardPile.push_back(card); // masuk discard
}

void SkillCardManager::dropCard(Player* player, int idx) {
    if (!isValidIndex(player, idx)) return;
    SkillCard* card = player->removeCard(idx);
    if (card != nullptr) {
        discardPile.push_back(card); // masuk discard, bisa reshuffle nanti
    }
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
    reshuffleIfEmpty();

    if (deck.empty()) return new MoveCard(1); // fallback
    SkillCard* card = deck.back();
    deck.pop_back();
    return card;
}