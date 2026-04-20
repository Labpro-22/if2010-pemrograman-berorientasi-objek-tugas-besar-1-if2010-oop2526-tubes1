#ifndef SKILLCARDMANAGER_H
#define SKILLCARDMANAGER_H

#include <cstddef>
#include <vector>
using namespace std;

class Player;
class SkillCard;
class GameContext;

class SkillCardManager {
private:
    size_t maxHandSize;
    bool isValidIndex(Player* player, int idx);

public:
    SkillCardManager(int maxSize);

    void distributeCardToAll(vector<Player*> players);
    void distributeCardTo(Player* player);
    void useCard(Player* player, int idx, GameContext* ctx);
    void dropCard(Player* player, int idx);

    void decrementDurations(Player* player);

    SkillCard* generateCard();
};

#endif
