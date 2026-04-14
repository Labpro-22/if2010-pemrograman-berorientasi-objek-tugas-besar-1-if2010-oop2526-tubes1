#pragma once

#include <string>
#include <vector>
using namespace std;

enum PLAYER_STATUS { ACTIVE, BANKRUPT, JAILED };
enum CARD_EFFECT { NOEFFECT, DISCOUNT, SHIELD };

class Player {
    private:
        string username;
        int currency;
        PLAYER_STATUS currentStatus;
        CardDeck<SkillCard> deck;
        Tile* currentTile;
        vector<Tile*> ownedProperties;
        float discountValue;
        int jailTurnCount;
        bool canUseCard;
    public:
        // getter

        string getUsername() const { return username; };
        int getCurrency() const { return currency; };
        PLAYER_STATUS getStatus() const { return currentStatus; };
        CardDeck<SkillCard> getDeck() const { return deck; };
        Tile* getCurrentTile() const { return currentTile; }
        vector<Tile*> getOwnedProperties() const { return ownedProperties; };
        float getDiscount() const { return discountValue; };
        int getJailTurn() const { return jailTurnCount; };
        bool canUseCard() const { return canUseCard; };
        
        // specific method

        void buyBackMortgaged(Tile* mortgaged);
        Player* operator+=(int money); // untuk proses penambahan currency
        Player* operator-=(int money); // untuk proses pengurangan currency
        void moveTo(Tile* destination, bool getPayment);
        void mortgageProperty(Tile* property); // ubah status Tile jadi mortgaged
};