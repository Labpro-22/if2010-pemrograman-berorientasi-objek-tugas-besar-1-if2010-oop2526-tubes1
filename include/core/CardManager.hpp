#pragma once

#include <memory>
#include "../../include/models/Deck.hpp"
#include "../../include/models/Card.hpp"
#include "../../include/models/Player.hpp" 
#include "../../include/models/Saveable.hpp"

class CardManager : Saveable {
private:
    Deck<std::unique_ptr<ActionCard>> chance_deck;
    Deck<std::unique_ptr<ActionCard>> community_chest_deck;
    Deck<std::unique_ptr<SkillCard>> skill_deck;

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
    void discardSkillCard(std::unique_ptr<SkillCard> usedCard);
    std::string toSaveFormat() const override;
};