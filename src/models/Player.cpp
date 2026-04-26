#include <algorithm>
#include <memory>
#include <sstream>
#include <iostream>
#include "../../include/models/Player.hpp"
#include "../../include/models/PropertyTile.hpp"
#include "../../include/models/SkillCard.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/core/PropertyManager.hpp"

std::random_device Player::rd;
std::mt19937 Player::gen(Player::rd());

Player::Player(std::string name, float balance, int position, PlayerState player_state): name(name), balance(balance), position(position), player_state(player_state) {}
std::string Player::getname() {
    return name;
}
Player& Player::operator+=(float amount){
    balance += amount;
    return *this;
}
Player& Player::operator-=(float amount){
    balance -= amount;
    return *this;
}
Player Player::operator+(float amount) const{
    Player result(name, balance, position, player_state);
    result.balance += amount;
    return result;
}
Player Player::operator-(float amount) const{
    Player result(name, balance, position, player_state);
    result.balance -= amount;
    return result;
}
void Player::receive(int amount){
    *this+=amount;
}
void Player::pay(int amount){
        *this-=amount;
}
void Player::declareBankruptcy(){
    this->player_state=PlayerState::BANKCRUPT;
}

void Player::transferTo(Player& player, float amount){
    int paid_amount=amount;
    for (const auto& effect : active_effects) {
        if (effect->blocksPayment()) {
            paid_amount= 0;
        }
    }
    balance -= paid_amount;
    player+=paid_amount;

}
float Player::getmoney(){
    return this->balance;
}
void Player::movePlayer(int steps){
    int size=PropertyManager::getBoard().getSize();
    position=(position+steps) % size;
}
void Player::addProperty(PropertyTile* property){
    this->owned_properties.push_back(property);
}
void Player::removeProperty(PropertyTile* property){
    owned_properties.erase(
        std::remove(owned_properties.begin(), owned_properties.end(), property),
        owned_properties.end()
    );
}
void Player::addSkillCard(std::unique_ptr<SkillCard> card){
    this->saved_cards.push_back(std::move(card));
}
void Player::useSkillCard(int index){
    this->saved_cards[index]->useEffect(*this);
}
void Player::addEffect(std::unique_ptr<Effect> effect){
    this->active_effects.push_back(std::move(effect));
}
float Player::getTotalAssetValue(){
    float sum=this->balance;
    for (auto& asset:this->owned_properties){
        sum+=asset->getBuyPrice();
    }
    return sum;
}
bool Player::canPay(float amount){
    return this->balance >= amount;
}
void Player::buyProperty(PropertyTile &property){
    float harga=property.getBuyPrice();
    for (auto &ef:this->active_effects){
        ef->modifyPayment(harga);
    }
    if(!this->canPay(harga)){
        throw "uang tak cukup";
    }
    *this-=harga;
    this->addProperty(&property);
    property.setPropertyStatus(PropertyStatus::OWNED);
    property.setPropertyOwner(shared_from_this());
}
void Player::sellProperty(PropertyTile &property){
    this->removeProperty(&property);
    *this+=property.getBuyPrice();
    property.setPropertyStatus(PropertyStatus::BANK);
}
float Player::liquidateAsset(float required){
    std::vector<PropertyTile*> properties_to_sell = owned_properties;
    std::sort(
        properties_to_sell.begin(),
        properties_to_sell.end(),
        [](PropertyTile* a, PropertyTile* b) {
            return a->getBuyPrice() < b->getBuyPrice();
        }
    );

    int liquidated = 0;
    for (PropertyTile* property : properties_to_sell) {
        if (liquidated >= required) {
            break;
        }

        int sell_value = property->getBuyPrice();
        liquidated += sell_value;
        balance += sell_value;
        removeProperty(property);
        property->setPropertyStatus(BANK);
        property->setPropertyOwner(std::shared_ptr<Player>());
    }

    return liquidated;
}
std::string Player::toSaveFormat() const {
    std::ostringstream out;
    out << name << " " << balance << " " << position;
    switch (player_state)
    {
    case PlayerState::FREE:
        out<<"FREE\n";
        break;
    case PlayerState::INJAIL:
        out<<"INJAIL\n";
        break;
    case PlayerState::BANKCRUPT:
        out<<"BANKCRUPT\n";
        break;
    }

    out << saved_cards.size() << "\n";

    
    // for(const std::unique_ptr<SkillCard>& card : saved_cards){
    //     card->
    // }


    return out.str();
}

bool Player::inJail() {
    return player_state == PlayerState::INJAIL;
}

void Player::setInJail() {
    player_state = PlayerState::INJAIL;
}

void Player::setFree() {
    player_state = PlayerState::FREE;
}

void Player::startTurn(int step) {
    auto& board = PropertyManager::getBoard();
    for (const auto& effect : active_effects) {
        effect->onTurnStart(*this);
    }
    // std::uniform_int_distribution<> dist(1, 100);
    this->movePlayer(step);
    board.getTile(this->position).onLand(*this);
}

void Player::endTurn() {
    for (const auto& effect : active_effects) {
        effect->onTurnEnd(*this);
    }

    active_effects.erase(
        std::remove_if(
            active_effects.begin(),
            active_effects.end(),
            [](const std::unique_ptr<Effect>& effect) {
                return effect->isExpired();
            }
        ),
        active_effects.end()
    );
}

int Player::getPosition() {
    return position;
}
int Player::countOwnedRailroad() const {
    return std::count_if(owned_properties.begin(), owned_properties.end(),
        [](PropertyTile* prop) {
            return prop->getPropertyType() == PropertyType::RAILROAD;
        });
}
int Player::countOwnedUtility() const {
    return std::count_if(owned_properties.begin(), owned_properties.end(),
        [](PropertyTile* prop) {
            return prop->getPropertyType() == PropertyType::UTILITY;
        });
}