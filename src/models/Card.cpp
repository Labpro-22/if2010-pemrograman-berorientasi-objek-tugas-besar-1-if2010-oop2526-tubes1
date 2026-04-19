#include "../../include/models/Card.hpp"

Card::Card(const std::string& description)
    : description(description) {}

std::string Card::getDescription() const {
    return description;
}
