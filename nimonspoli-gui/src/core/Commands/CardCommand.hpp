#pragma once
#include "./Command.hpp"
#include "../Player/Player.hpp"
#include "../Card/Card.hpp"
#include "../Card/CardDeck.hpp"
#include "../GameState/GameState.hpp"
#include "../utils/TransactionLogger.hpp"
#include "../Exceptions/FinancialException.hpp"
#include <iostream>

class CardCommand : public Command
{
private:
    Player *p;
    CardDeck<Card> *deck;
    GameState *gs;
    TransactionLogger *logger;
    int turnNumber;
    std::string deckLabel; 

public:
    CardCommand(Player *p, CardDeck<Card> *deck, GameState *gs,
                TransactionLogger *logger, int turnNumber,
                const std::string &deckLabel);
    void execute(GameMaster& ) override;
};