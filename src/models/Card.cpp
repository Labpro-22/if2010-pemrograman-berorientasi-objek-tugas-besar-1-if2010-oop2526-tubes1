#include "../../include/models/Card.hpp"

Card::Card() = default;

std::random_device Card::rd;
std::mt19937 Card::gen(Card::rd());