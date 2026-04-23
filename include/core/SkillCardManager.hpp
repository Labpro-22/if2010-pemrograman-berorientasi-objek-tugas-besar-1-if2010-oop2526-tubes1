#ifndef SKILLCARDMANAGER_H
#define SKILLCARDMANAGER_H

#include <cstddef>
#include <vector>

#include "../utils/CardDeck.hpp"
using namespace std;

class Player;
class SkillCard;
class GameContext;

class SkillCardManager {
private:
    size_t maxHandSize;
    CardDeck<SkillCard*> skillDeck;

    bool isValidIndex(Player* player, int idx);
    void populateDeck(size_t cardCount);

public:
    SkillCardManager(int maxSize);
    ~SkillCardManager();

    void distributeCardToAll(vector<Player*> players);
    void distributeCardTo(Player* player);
    void useCard(Player* player, int idx, GameContext* ctx);
    void dropCard(Player* player, int idx);

    void decrementDurations(Player* player);

    SkillCard* generateCard();
};

#endif
