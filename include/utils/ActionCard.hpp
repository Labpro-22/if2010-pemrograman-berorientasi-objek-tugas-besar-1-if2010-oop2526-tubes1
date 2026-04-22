#ifndef ACTIONCARD_H
#define ACTIONCARD_H

#include <string>
using namespace std;

class Player;
class GameContext;

enum class ActionCardType {
    GAIN_MONEY,
    PAY_MONEY,
    MOVE,
    TELEPORT,
    GO_TO_JAIL,
    GET_SKILL
};

class ActionCard {
public:
    virtual void execute(Player* player, GameContext* ctx) = 0;
    virtual string getDescription() = 0;
    virtual ~ActionCard() {}
};

class ChanceCard : public ActionCard {
private:
    ActionCardType type;
    int value;
    string desc;

public:
    ChanceCard(ActionCardType t, int v, string d);

    void execute(Player* player, GameContext* ctx) override;
    string getDescription() override;
};

class CommunityCard : public ActionCard {
private:
    ActionCardType type;
    int value;
    string desc;

public:
    CommunityCard(ActionCardType t, int v, string d);

    void execute(Player* player, GameContext* ctx) override;
    string getDescription() override;
};

#endif
