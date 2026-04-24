#include <algorithm>
#include <memory>
#include <sstream>
#include "../models/Player.hpp"
#include "../models/PropertyTile.hpp"
#include "../models/SkillCard.hpp"
#include "../models/Board.hpp"

Player::Player(std::string name, int balance, int position, PlayerState player_state):name(name),balance(balance),position(position){}
Player& Player::operator+=(int amount){
    balance += amount;
    return *this;
}
Player& Player::operator-=(int amount){
    balance -= amount;
    return *this;
}
Player Player::operator+(int amount) const{
    Player result = *this;
    result.balance += amount;
    return result;
}
Player Player::operator-(int amount) const{
    Player result = *this;
    result.balance -= amount;
    return result;
}
void Player::transferTo(Player& player, int amount){
    int paid_amount=amount;
    for (const auto& effect : active_effects) {
        if (effect->blocksPayment()) {
            paid_amount= 0;
        }
    }
    balance -= paid_amount;
    player+=paid_amount;
}
void Player::movePlayer(int steps){
    position+=steps;
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
void Player::useSkillCard(int index,Board& b, std::vector<Player>& all, TileVisitor& visitor){
    this->saved_cards[index]->useEffect(*this,  b, all, visitor);
}
void Player::addEffect(std::unique_ptr<Effect> effect){
    this->active_effects.push_back(std::move(effect));
}
int Player::getTotalAssetValue(){
    int sum=this->balance;
    for (auto& asset:this->owned_properties){
        sum+=asset->getSellValue();
    }
    return sum;
}
bool Player::canPay(int amount){
    return this->balance >= amount;
}
void Player::buyProperty(PropertyTile &property){
    float harga=property.getSellValue();
    for (auto &ef:this->active_effects){
        ef->modifyPayment(harga);
    }
    if(!this->canPay(harga)){
        throw "uang tak cukup";
    }
    *this-=harga;
    this->addProperty(&property);
    property.setPropertyOwner(std::make_shared<Player>(this));
}
void Player::sellProperty(PropertyTile &property, Player& other){
    this->removeProperty(&property);
    *this+=property.getSellValue();
    other.buyProperty(property);
}
int Player::liquidateAsset(int required){
    std::vector<PropertyTile*> properties_to_sell = owned_properties;
    std::sort(
        properties_to_sell.begin(),
        properties_to_sell.end(),
        [](PropertyTile* a, PropertyTile* b) {
            return a->getSellValue() < b->getSellValue();
        }
    );

    int liquidated = 0;
    for (PropertyTile* property : properties_to_sell) {
        if (liquidated >= required) {
            break;
        }

        int sell_value = property->getSellValue();
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

void Player::startTurn(Board &board) {
    for (const auto& effect : active_effects) {
        effect->onTurnStart(*this);
    }
    this->movePlayer();
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