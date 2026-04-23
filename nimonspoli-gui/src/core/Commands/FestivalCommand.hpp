#pragma once
#include "./Command.hpp"
#include "../Player/Player.hpp"
#include "../Property/StreetProperty.hpp"
#include "../utils/TransactionLogger.hpp"

class FestivalCommand : public Command
{
private:
    Player *p;
    TransactionLogger *logger;
    int turnNumber;

public:
    FestivalCommand(Player *p, TransactionLogger *logger, int turnNumber);
    void execute(GameMaster& gm) override;

private:
    StreetProperty *promptSelectProperty();
    void applyFestival(StreetProperty *prop);
};