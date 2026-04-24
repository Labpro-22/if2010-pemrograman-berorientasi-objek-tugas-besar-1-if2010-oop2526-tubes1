#include "models/Player/Player.hpp"

#include <algorithm>
#include <utility>
#include "core/GameException.hpp"
#include "core/SkillContext.hpp"
#include "models/Card/SkillCard/SkillCard.hpp"
#include "models/Plot/Plot.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

Player::Player()
    : username(""),
      cash(0),
      position(0),
      status(PlayerStatus::ACTIVE),
      jailTurns(0),
      hasRolled(false),
      shieldActive(false),
      usedSkillThisTurn(false),
      shieldTurnLeft(0),
      discountTurnLeft(0),
      discountValue(0),
      consecutiveDoubles(0) {}

Player::Player(std::string username, int startingCash, int startPosition)
    : username(std::move(username)),
      cash(startingCash),
      position(startPosition),
      status(PlayerStatus::ACTIVE),
      jailTurns(0),
      hasRolled(false),
      shieldActive(false),
      usedSkillThisTurn(false),
      shieldTurnLeft(0),
      discountTurnLeft(0),
      discountValue(0),
      consecutiveDoubles(0) {
    if (startingCash < 0) {
        throw InvalidInputException("startingCash tidak boleh negatif.");
    }
}

const std::string& Player::getUsername() const {
    return username;
}

int Player::getCash() const {
    return cash;
}

int Player::getPosition() const {
    return position;
}

PlayerStatus Player::getStatus() const {
    return status;
}

int Player::getJailTurns() const {
    return jailTurns;
}

const std::vector<std::reference_wrapper<Plot>>& Player::getOwnedProperties() const {
    return ownedProperties;
}

const std::vector<std::shared_ptr<SkillCard>>& Player::getOwnedCards() const {
    return ownedCards;
}

bool Player::getHasRolled() const {
    return hasRolled;
}

bool Player::isShieldActive() const {
    return shieldActive;
}

bool Player::hasUsedSkillThisTurn() const {
    return usedSkillThisTurn;
}

int Player::getShieldTurnLeft() const {
    return shieldTurnLeft;
}

int Player::getDiscountTurnLeft() const {
    return discountTurnLeft;
}

int Player::getDiscountValue() const {
    return discountValue;
}

int Player::getConsecutiveDoubles() const {
    return consecutiveDoubles;
}

int Player::getTotalWealth() const {
    int wealth = cash;
    for (const std::reference_wrapper<Plot>& propertyRef : ownedProperties) {
        const Plot& property = propertyRef.get();
        if (const auto* land = dynamic_cast<const LandPlot*>(&property)) {
            wealth += land->getBuyPrice(); //TODO: hindari penggunaan dynamic cast dan hitung harga bangunan
            continue;
        }

        if (const auto* genericProperty = dynamic_cast<const PropertyPlot*>(&property)) {
            wealth += genericProperty->getMortgageValue();
        }
    }
    return wealth;
}

void Player::move() {
    move(1, 40); //TODO: Sesuaikan dengan dynamic board
}

void Player::move(int steps, int boardSize) {
    if (boardSize <= 0) {
        throw InvalidInputException("boardSize harus lebih dari 0.");
    }

    int next = (position + steps) % boardSize;
    if (next < 0) {
        next += boardSize;
    }
    position = next;
}

void Player::moveTo(int index, int boardSize) {
    if (boardSize <= 0) { 
        throw InvalidInputException("boardSize harus lebih dari 0.");
    }
    if (index < 0 || index >= boardSize) {
        throw OutOfRangeException("Posisi tujuan di luar board.");
    }

    position = index;
}

void Player::pay(int amount) {
    if (amount < 0) {
        throw InvalidInputException("Nilai pembayaran tidak boleh negatif.");
    }
    if (cash < amount) {
        throw InsufficientFundException();
    }
    cash -= amount;
}

void Player::payTaxes() {
    if (shieldActive) {
        shieldActive = false;
        return;
    }
    pay(cash / 10); //FIXME
}

bool Player::buyProperty(PropertyPlot& property) { //TODO: ubah jadi void
    if (property.getOwner() == this) {
        return false; //TODO: throw exception
    }

    int price = property.getBuyPrice();

    pay(price);
    ownedProperties.push_back(property);
    property.setOwner(this);
    return true; //TODO: hapus jika return void
}

bool Player::useCards(std::size_t cardIndex, SkillContext& ctx) {
    if (usedSkillThisTurn || cardIndex >= ownedCards.size()) {
        return false;
    }

    std::shared_ptr<SkillCard> selectedCard = ownedCards[cardIndex];
    ownedCards.erase(ownedCards.begin() + static_cast<std::vector<std::shared_ptr<SkillCard>>::difference_type>(cardIndex));
    if (!selectedCard) {
        return false;
    }

    selectedCard->activate(ctx);
    usedSkillThisTurn = true;
    return true;
}

bool Player::dropCard() {
    if (ownedCards.empty()) {
        return false;
    }
    return dropCard(ownedCards.size() - 1);
}

bool Player::dropCard(std::size_t cardIndex) {
    if (cardIndex >= ownedCards.size()) {
        return false;
    }

    ownedCards.erase(ownedCards.begin() + static_cast<std::vector<std::shared_ptr<SkillCard>>::difference_type>(cardIndex));
    return true;
}

void Player::receive(int amount) {
    if (amount < 0) {
        throw InvalidInputException("Nilai penerimaan tidak boleh negatif.");
    }
    cash += amount;
}

Player& Player::operator+=(int amount) {
    receive(amount);
    return *this;
}

Player& Player::operator-=(int amount) {
    pay(amount);
    return *this;
}

bool Player::operator<(Player& other) {
    return getTotalWealth() < other.getTotalWealth();
}

bool Player::operator>(Player& other) {
    return getTotalWealth() > other.getTotalWealth();
}

void Player::setStatus(PlayerStatus newStatus) {
    status = newStatus;
    if (newStatus == PlayerStatus::BANKRUPT) {
        cash = 0;
        jailTurns = 0;
        hasRolled = false;
        shieldActive = false;
        usedSkillThisTurn = false;
        shieldTurnLeft = 0;
        discountTurnLeft = 0;
        discountValue = 0;
        consecutiveDoubles = 0;
        ownedCards.clear();
        ownedProperties.clear();
    }
}

void Player::setJailTurns(int turns) {
    if (turns < 0) {
        throw InvalidInputException("Jumlah giliran penjara tidak boleh negatif.");
    }
    jailTurns = turns;
}

void Player::setHasRolled(bool value) {
    hasRolled = value;
}

void Player::setShieldActive(bool value) {
    shieldActive = value;
}

void Player::setShieldTurnLeft(int turns) {
    if (turns < 0) {
        throw InvalidInputException("shieldTurnLeft tidak boleh negatif.");
    }
    shieldTurnLeft = turns;
}

void Player::setDiscountTurnLeft(int turns) {
    if (turns < 0) {
        throw InvalidInputException("discountTurnLeft tidak boleh negatif.");
    }
    discountTurnLeft = turns;
}

void Player::setDiscountValue(int value) {
    if (value < 0) {
        throw InvalidInputException("discountValue tidak boleh negatif.");
    }
    discountValue = value;
}

void Player::decrementJailTurns() {
    if (jailTurns > 0) {
        --jailTurns;
    }
}

void Player::decrementShieldTurn() {
    if (shieldTurnLeft > 0) {
        --shieldTurnLeft;
    }
}

void Player::decrementDiscountTurn() {
    if (discountTurnLeft > 0) {
        --discountTurnLeft;
        if (discountTurnLeft == 0) {
            discountValue = 0;
        }
    }
}

void Player::incrementConsecutiveDoubles() {
    ++consecutiveDoubles;
}

void Player::resetConsecutiveDoubles() {
    consecutiveDoubles = 0;
}

void Player::addOwnedCard(const std::shared_ptr<SkillCard>& card) {
    if (!card) {
        throw InvalidInputException("Card tidak boleh null.");
    }
    ownedCards.push_back(card);
}

void Player::setUsedSkillThisTurn(bool used) {
    usedSkillThisTurn = used;
}

void Player::resetTurnFlags() {
    usedSkillThisTurn = false;
    hasRolled = false;
    shieldActive = false;
}

int Player::countOwnedStation() const {
    int count;
    for (auto property : ownedProperties){
        if (property.get().getType() == PlotType::STATIONPLOT){
            count++;
        }
    }
    return count;
}

int Player::countOwnedUtility() const {
    int count;
    for (auto property : ownedProperties){
        if (property.get().getType() == PlotType::UTILITYPLOT){
            count++;
        }
    }
    return count;
}

bool Player::isBankrupt() const {
    return status == PlayerStatus::BANKRUPT;
}
