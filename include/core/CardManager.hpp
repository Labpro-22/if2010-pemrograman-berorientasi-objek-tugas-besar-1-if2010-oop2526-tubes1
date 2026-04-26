#pragma once
#include "../models/DrawCardDeck.hpp"
#include "../models/ChanceCard.hpp"
#include "../models/ChanceCard.hpp"
#include "../models/CommunityChestCard.hpp"
#include "../models/AbilityCardDeck.hpp"
#include "../data-layer/GameStateLoader.hpp"
#include "../data-layer/GameStateSaver.hpp"

class CardManager
{
private:
    DrawCardDeck<ChanceCard> chanceDeck;
    DrawCardDeck<CommunityChestCard> communityDeck;
    AbilityCardDeck abilityDeck;

public:
    CardManager();
    CardManager(
        DrawCardDeck<ChanceCard> chanceDeck,
        DrawCardDeck<CommunityChestCard> communityDeck,
        AbilityCardDeck&& abilityDeckDrawCardDeck);
    ChanceCard* drawChanceCard();
    CommunityChestCard* drawCommunityCard();
    std::unique_ptr<AbilityCard> drawAbilityCard();
    void giveTurnStartAbility(Player* player);
    void discardAbilityCard(std::unique_ptr<AbilityCard> card);
    DrawCardDeck<ChanceCard>& getChanceDeck() { return chanceDeck; }
    DrawCardDeck<CommunityChestCard>& getCommunityDeck() { return communityDeck; }
    AbilityCardDeck& getAbilityDeck() { return abilityDeck; }
    void setChanceDeck(DrawCardDeck<ChanceCard> chanceDeck);
    void setCommunityDeck(DrawCardDeck<CommunityChestCard> comunityDeck);
    void setAbilityDeck(AbilityCardDeck&& abilityDeck);
};
