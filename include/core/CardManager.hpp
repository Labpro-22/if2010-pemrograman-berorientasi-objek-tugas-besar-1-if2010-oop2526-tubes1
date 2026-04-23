#pragma once

#include <memory>
#include "Deck.hpp"
#include "Card.hpp"
#include "Player.hpp" 
#include "Saveable.hpp"

class CardManager : Saveable {
private:
    Deck<Card*> chance_deck;
    Deck<Card*> community_chest_deck;
    Deck<Card*> skill_deck;

public:
    CardManager();
    ~CardManager() = default;

    // Membaca file konfigurasi dan mengisi ketiga deck di atas
    void initializeDecks();

    // Ketika pemain mendarat di petak Kesempatan
    void drawKesempatan(Player& player);

    // Ketika pemain mendarat di petak Dana Umum
    void drawDanaUmum(Player& player);

    // Memberikan Skill Card ke pemain
    void giveSkillCardToPlayer(Player& player);

    // Fungsi ini dipanggil oleh GameManager ketika pemain selesai menggunakan SkillCard.
    void discardSkillCard(SkillCard* usedCard);
    std::string toSaveFormat();
};