#pragma once

#include "ActionCard.hpp"
#include <random>

enum class transaction_type {PLAYER_TO_BANK, BANK_TO_PLAYER, PLAYER_TO_ALL, ALL_TO_PLAYER};
class FinancialActionCard : public ActionCard { 
    private:
        transaction_type t;
        float amount;
    public:
        FinancialActionCard();
        /**
         * 
         * @brief 
         * 
         * @param p Target player 
         * @param b Board
         * @param all All players
         */
        void onDraw(Player& p) override;


};