#include "../../include/models/SkillCard.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Effect.hpp"
#include "../../include/core/GameManager.hpp"
#include "../../include/core/PropertyManager.hpp"
#include <random>
#include <limits>
#include <sstream>
#include <iostream>
// SkillCard
SkillCard::SkillCard(std::string name):name(name){};
void SkillCard::onDraw(Player& p) {
}
std::string SkillCard::toSaveFormat() const{
    std::ostringstream out;
    out << name << "\n";
    return out.str();
}

// MoveSkillCard
MoveSkillCard::MoveSkillCard(std::string name):SkillCard(name){}
void MoveSkillCard::onDraw(Player& p) {
    std::uniform_int_distribution<> dist(1, 100);
    this->step = dist(gen);
}
void MoveSkillCard::useEffect(Player& p) {
    p.movePlayer(this->step);
}
std::string MoveSkillCard::toSaveFormat() const{
    std::ostringstream out;
    out << name << " " << step << "\n";
    return out.str();
}

// DiscountSkillCard
DiscountSkillCard::DiscountSkillCard(std::string name):SkillCard(name){}
void DiscountSkillCard::onDraw(Player& p) {
    std::uniform_real_distribution<> dist(1, 100);
    this->percentage = dist(gen);
}

void DiscountSkillCard::useEffect(Player& p) {
    p.addEffect(std::unique_ptr<Effect>(new DiscountEffect(this->percentage)));
}
std::string DiscountSkillCard::toSaveFormat() const{
    std::ostringstream out;
    out << name << " " << percentage << "\n";
    return out.str();
}


// ShieldSkillCard
ShieldSkillCard::ShieldSkillCard(std::string name):SkillCard(name){}
void ShieldSkillCard::useEffect(Player& p) {
    p.addEffect(std::unique_ptr<Effect>(new ShieldEffect()));
}

// TeleportSkillCard
TeleportSkillCard::TeleportSkillCard(std::string name):SkillCard(name){}
void TeleportSkillCard::useEffect(Player& p) {
    int steps = 0;
    p.movePlayer(steps);
}

// LassoSkillCard
LassoSkillCard::LassoSkillCard(std::string name):SkillCard(name){}
void LassoSkillCard::useEffect(Player& p) {
    auto &all=GameManager::players;
    const int boardSize = PropertyManager::getBoard().getSize();
    int myPos = p.getPosition();
    Player* moved = nullptr;
    int distance = std::numeric_limits<int>::max();
    for (auto& pl : all) {
        if (&p == pl.get()) {
            continue;
        }
        int targetPos = pl->getPosition() % boardSize;
        if (targetPos < 0) {
            targetPos += boardSize;
        }
        int temp = (targetPos - myPos + boardSize) % boardSize;
        if (temp > 0 && temp < distance) {
            distance = temp;
            moved = pl.get();
        }
    }if (moved) {
        moved->movePlayer(-distance);
    }
}


// DemolitionSkillCard
DemolitionSkillCard::DemolitionSkillCard(std::string name):SkillCard(name){}
void DemolitionSkillCard::useEffect(Player& p) {
    // TODO: Implement demolition skill card effect
}
