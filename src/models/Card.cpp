#include "../../include/models/Card.hpp"
#include "../../include/core/Player.hpp"
#include "../../include/models/Board.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/core/GameEngine.hpp"
#include "../../include/core/Exceptions.hpp"
#include <iostream>
#include <random>

//random integer
static int randomInt(int lo, int hi) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}


SkillCard::SkillCard(const std::string& name,
                     const std::string& description,
                     SkillCardType type)
    : cardName(name), cardDescription(description), skillType(type) {}

SkillCard::~SkillCard() {}

void SkillCard::use(Player& player, GameEngine& engine) {

}

std::string SkillCard::getCardName()const {
    return cardName; 
}
std::string SkillCard::getCardDescription()const {
    return cardDescription;
}
SkillCardType SkillCard::getSkillType()const {
    return skillType; 
}



// MoveCard , majunya dirandom

MoveCard::MoveCard()
    : SkillCard("MoveCard",
                "Maju sejumlah petak. Langkah ditentukan acak saat kartu didapat.",
                SkillCardType::MOVE),
      steps(randomInt(1, 6)) {}

//load dari saved
MoveCard::MoveCard(int savedSteps)
    : SkillCard("MoveCard",
                "Maju sejumlah petak. Langkah ditentukan acak saat kartu didapat.",
                SkillCardType::MOVE),
      steps(savedSteps) {}

int MoveCard::getSteps() const {
    return steps; 
}


//BELOM ADA GETTERNYEEE PLAYER
void MoveCard::use(Player& player, GameEngine& engine) {
    std::cout << "[MoveCard] " << player.getUsername()
              << " menggunakan MoveCard — maju " << steps << " petak.\n";
    player.move(steps); 
}

std::string MoveCard::getValueString() const {
    return std::to_string(steps);
}



static int randomDiscountPercent() {
    return randomInt(1, 5) * 10; 
}

DiscountCard::DiscountCard()
    : SkillCard("DiscountCard",
                "Diskon persentase acak pada sewa/pajak selama 1 giliran.",
                SkillCardType::DISCOUNT),
      discountPercent(randomDiscountPercent()),
      remainingTurns(1) {}

//load dari save file
DiscountCard::DiscountCard(int savedPercent, int savedTurns)
    : SkillCard("DiscountCard",
                "Diskon persentase acak pada sewa/pajak selama 1 giliran.",
                SkillCardType::DISCOUNT),
      discountPercent(savedPercent),
      remainingTurns(savedTurns) {}

int  DiscountCard::getDiscountPercent() const {
    return discountPercent;
}
int  DiscountCard::getRemainingTurns() const {
    return remainingTurns; 
}
void DiscountCard::decrementTurns(){ 
    if (remainingTurns > 0) --remainingTurns; }

// Daftarkan kartu aktif ke player, GameEngine membaca saat menghitung sewa/pajak
void DiscountCard::use(Player& player, GameEngine& engine) {
    player.setActiveDiscountPercent(discountPercent); 
    std::cout << "[DiscountCard] " << player.getUsername()
              << " menggunakan DiscountCard — diskon " << discountPercent
              << "% berlaku selama giliran ini.\n";
}

std::string DiscountCard::getValueString() const {
    return std::to_string(discountPercent);
}



ShieldCard::ShieldCard()
    : SkillCard("ShieldCard",
                "Kebal tagihan atau sanksi apapun selama 1 giliran.",
                SkillCardType::SHIELD) {}

void ShieldCard::use(Player& player, GameEngine& engine) {
    player.setShieldActive(true);
    std::cout << "[ShieldCard] " << player.getUsername()
              << " menggunakan ShieldCard — kebal terhadap tagihan/sanksi giliran ini.\n";
}

std::string ShieldCard::getValueString() const {
    return ""; // ShieldCard tidak memiliki nilai numerik
}



TeleportCard::TeleportCard()
    : SkillCard("TeleportCard",
                "Pindah ke petak manapun di papan permainan.",
                SkillCardType::TELEPORT) {}

void TeleportCard::use(Player& player, GameEngine& engine) {
    std::cout << "Masukkan kode petak tujuan: ";
    std::string kode;
    std::cin >> kode;
    
    Board* board = Board::getInstance();
    Tile* target = board->getTileByKode(kode);
    if (!target) {
        std::cout << "Petak tidak ditemukan!\n";
        return;
    }
    int dest = target->getIndex() - 1;
    int curr = player.getPosition();
    int steps = (dest - curr + 40) % 40;
    player.move(steps);
    std::cout << player.getUsername() << " teleport ke " 
              << target->getName() << ".\n";
}

std::string TeleportCard::getValueString() const {
    return ""; // TeleportCard tidak memiliki nilai numerik
}



// LassoCard, tarik pemain lawan yang adad di depan ke posisi kita saat ini


LassoCard::LassoCard()
    : SkillCard("LassoCard",
                "Tarik satu pemain lawan di depan posisimu ke petakmu saat ini.",
                SkillCardType::LASSO) {}

void LassoCard::use(Player& player, GameEngine& engine) {
    std::cout << "[LassoCard] " << player.getUsername()
              << " menggunakan LassoCard.\n";
    // GameEngine mencari target (lawan paling dekat di depan) dan memindahkannya
    // engine.handleLasso(player); TODO: endra
}

std::string LassoCard::getValueString() const {
    return ""; // LassoCard tidak memiliki nilai numerik
}


// DemolitionCard, ancur2in properti lawan

DemolitionCard::DemolitionCard()
    : SkillCard("DemolitionCard",
                "Hancurkan 1 bangunan (rumah/hotel) pada properti milik lawan.",
                SkillCardType::DEMOLITION) {}

void DemolitionCard::use(Player& player, GameEngine& engine) {
    std::cout << "[DemolitionCard] " << player.getUsername()
              << " menggunakan DemolitionCard.\n";
    // GameEngine meminta pemain memilih target properti lawan, lalu menghancurkan bangunan
    // engine.handleDemolition(player);   TODO: endra
}

std::string DemolitionCard::getValueString() const {
    return ""; 
}



ActionCard::ActionCard(const std::string& name, const std::string& description)
    : cardName(name), cardDescription(description) {}

ActionCard::~ActionCard() {}

void ActionCard::execute(Player& player, GameEngine& engine) {}

std::string ActionCard::getCardName() const {
    return cardName; 
}
std::string ActionCard::getCardDescription() const {
    return cardDescription;
}


// ChanceCard , abis dipake taro ke tumpukan

ChanceCard::ChanceCard(ChanceCardType cardType)
    : ActionCard(
          cardType == ChanceCardType::NEAREST_STATION ? "Pergi ke Stasiun Terdekat" :
          cardType == ChanceCardType::MOVE_BACK_3      ? "Mundur 3 Petak"            :
                                                         "Masuk Penjara",
          cardType == ChanceCardType::NEAREST_STATION ? "Pergi ke stasiun terdekat." :
          cardType == ChanceCardType::MOVE_BACK_3      ? "Mundur 3 petak."            :
                                                         "Masuk Penjara."
      ),
      type(cardType) {}

ChanceCardType ChanceCard::getType() const { return type; }

void ChanceCard::execute(Player& player, GameEngine& engine) {
    std::cout << "Kartu: \"" << cardDescription << "\"\n";

    switch (type) {
        case ChanceCardType::NEAREST_STATION:
            // Pindah ke stasiun railroad terdekat, searah jarum jam dari posisi saat ini
            // engine.moveToNearestStation(player); TODO: endra
            break;

        case ChanceCardType::MOVE_BACK_3:
            player.move(-3); 
            break;

        case ChanceCardType::GO_TO_JAIL:
            player.goToJail();
            break;
    }
}


static const int BIRTHDAY_AMOUNT  = 100;
static const int DOCTOR_AMOUNT    = 700;
static const int ELECTION_AMOUNT  = 200;

CommunityChestCard::CommunityChestCard(CommunityChestCardType cardType)
    : ActionCard(
          cardType == CommunityChestCardType::BIRTHDAY  ? "Hadiah Ulang Tahun" :
          cardType == CommunityChestCardType::DOCTOR     ? "Biaya Dokter"        :
                                                           "Sumbangan Pemilu",
          cardType == CommunityChestCardType::BIRTHDAY  ?
              "Ini adalah hari ulang tahun Anda. Dapatkan M100 dari setiap pemain." :
          cardType == CommunityChestCardType::DOCTOR     ?
              "Biaya dokter. Bayar M700."                                            :
              "Anda mau nyaleg. Bayar M200 kepada setiap pemain."
      ),
      type(cardType) {}

CommunityChestCardType CommunityChestCard::getType() const { return type; }

void CommunityChestCard::execute(Player& player, GameEngine& engine) {
    std::cout << "Kartu: \"" << cardDescription << "\"\n";

    switch (type) {
        // semua pemain bayar BIRTHDAY_AMOUNT ke player ini
        case CommunityChestCardType::BIRTHDAY:
        
            //engine.handleBirthday(player, BIRTHDAY_AMOUNT); TODO: endra
            break;

        // pemain ini harus bayar DOCTOR_AMOUNT
        case CommunityChestCardType::DOCTOR:
            try {
                player -= DOCTOR_AMOUNT; // operator-= dari Player.hpp
                std::cout << player.getUsername() << " membayar M" << DOCTOR_AMOUNT
                          << " ke Bank. Sisa Uang = M" << player.getMoney() << ".\n";
            } catch (const NotEnoughMoneyException&) {
                std::cout << "Kamu tidak mampu membayar biaya dokter! (M"
                          << DOCTOR_AMOUNT << ")\n"
                          << "Uang kamu saat ini: M" << player.getMoney() << "\n";
                
                // engine.handleBankruptcy(player, nullptr, DOCTOR_AMOUNT); TODO: endra
            }
            break;

        case CommunityChestCardType::ELECTION:
            // engine.handleElection(player, ELECTION_AMOUNT); TODO: endra
            break;
    }
}