#pragma once

#include "Card.hpp"

class ActionCard : public Card {
public:
    explicit ActionCard(const std::string& description);
    ~ActionCard() override = default;
};
