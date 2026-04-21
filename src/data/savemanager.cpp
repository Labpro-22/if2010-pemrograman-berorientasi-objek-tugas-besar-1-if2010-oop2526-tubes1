#include "data/SaveManager.h"
#include "core/PropertyTypes.h"
#include "core/Cards.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>

namespace Nimonspoli {

// ── Save ──────────────────────────────────────────────────────────────────────
void SaveManager::save(const Game& game, const std::string& path) {
    std::ofstream f(path);
    if (!f) throw std::runtime_error("Gagal menyimpan file: " + path);

    // Line 1: <TURN_SAAT_INI> <MAX_TURN>
    f << game.currentTurn() << " " << game.maxTurn() << "\n";

    // Line 2: <JUMLAH_PEMAIN>
    f << game.players().size() << "\n";

    // Player states
    f << serializePlayers(game);

    // Turn order — write usernames of active players in order
    auto active = game.activePlayers();
    for (auto* p : active)
        f << p->username() << " ";
    f << "\n";

    // Current active player
    f << game.currentPlayer().username() << "\n";

    // Property states
    f << serializeProperties(game);

    // Deck state
    f << serializeDeck(game);

    // Log
    f << serializeLog(game);
}

std::string SaveManager::serializePlayers(const Game& game) {
    std::ostringstream ss;
    for (auto& p : game.players()) {
        // <USERNAME> <UANG> <POSISI_PETAK> <STATUS>
        std::string status;
        switch (p->status()) {
            case PlayerStatus::ACTIVE:   status = "ACTIVE";   break;
            case PlayerStatus::JAILED:   status = "JAILED";   break;
            case PlayerStatus::BANKRUPT: status = "BANKRUPT"; break;
        }
        std::string posCode = game.board().getTile(p->position())->code();
        ss << p->username() << " " << p->balance() << " "
           << posCode << " " << status << "\n";

        // <JUMLAH_KARTU_TANGAN>
        ss << p->handSize() << "\n";
        for (auto* card : p->hand()) {
            std::string type;
            int value = card->value();
            int dur   = card->remainingDuration();
            switch (card->skillType()) {
                case SkillCardType::MOVE:        type = "MoveCard";        break;
                case SkillCardType::DISCOUNT:    type = "DiscountCard";    break;
                case SkillCardType::SHIELD:      type = "ShieldCard";      break;
                case SkillCardType::TELEPORT:    type = "TeleportCard";    break;
                case SkillCardType::LASSO:       type = "LassoCard";       break;
                case SkillCardType::DEMOLITION:  type = "DemolitionCard";  break;
            }
            ss << type;
            // value only for MoveCard and DiscountCard
            if (card->skillType() == SkillCardType::MOVE ||
                card->skillType() == SkillCardType::DISCOUNT)
                ss << " " << value;
            // duration only for DiscountCard
            if (card->skillType() == SkillCardType::DISCOUNT)
                ss << " " << dur;
            ss << "\n";
        }
    }
    return ss.str();
}

std::string SaveManager::serializeProperties(const Game& game) {
    std::ostringstream ss;
    const auto& props = game.board().properties();
    ss << props.size() << "\n";

    for (auto& [code, prop] : props) {
        std::string jenis, owner, status;
        int fmult = 1, fdur = 0;
        char buildChar = '0';

        switch (prop->type()) {
            case PropertyType::STREET:   jenis = "street";   break;
            case PropertyType::RAILROAD: jenis = "railroad"; break;
            case PropertyType::UTILITY:  jenis = "utility";  break;
        }
        owner  = prop->owner() ? prop->owner()->username() : "BANK";
        switch (prop->status()) {
            case PropertyStatus::BANK:       status = "BANK";       break;
            case PropertyStatus::OWNED:      status = "OWNED";      break;
            case PropertyStatus::MORTGAGED:  status = "MORTGAGED";  break;
        }

        if (prop->type() == PropertyType::STREET) {
            auto* s = static_cast<Street*>(prop.get());
            fmult = s->festival().multiplier();
            fdur  = s->festival().duration();
            int lvl = s->buildingLevel();
            if (lvl == Street::HOTEL)
                buildChar = 'H';
            else
                buildChar = '0' + lvl;
        }

        ss << code << " " << jenis << " " << owner << " " << status
           << " " << fmult << " " << fdur << " " << buildChar << "\n";
    }
    return ss.str();
}

std::string SaveManager::serializeDeck(const Game& game) {
    std::ostringstream ss;
    auto cards = game.skillDeck().peekDrawPile();
    ss << cards.size() << "\n";
    for (auto* c : cards) {
        switch (static_cast<SkillCard*>(c)->skillType()) {
            case SkillCardType::MOVE:       ss << "MoveCard\n";       break;
            case SkillCardType::DISCOUNT:   ss << "DiscountCard\n";   break;
            case SkillCardType::SHIELD:     ss << "ShieldCard\n";     break;
            case SkillCardType::TELEPORT:   ss << "TeleportCard\n";   break;
            case SkillCardType::LASSO:      ss << "LassoCard\n";      break;
            case SkillCardType::DEMOLITION: ss << "DemolitionCard\n"; break;
        }
    }
    return ss.str();
}

std::string SaveManager::serializeLog(const Game& game) {
    std::ostringstream ss;
    const auto& entries = game.logger().entries();
    ss << entries.size() << "\n";
    for (auto& e : entries)
        ss << e.turn << " " << e.username << " " << e.action << " " << e.detail << "\n";
    return ss.str();
}

// ── Load ──────────────────────────────────────────────────────────────────────
bool SaveManager::load(Game& game, const std::string& path) {
    std::ifstream f(path);
    if (!f) return false;

    try {
        int turn, maxTurn;
        f >> turn >> maxTurn;

        int numPlayers;
        f >> numPlayers;
        f.ignore();

        deserializePlayers(game, f);
        // Turn order and active player
        std::string line;
        std::getline(f, line); // turn order line (space-separated usernames)
        std::getline(f, line); // current player username

        deserializeProperties(game, f);
        deserializeDeck(game, f);
        deserializeLog(game, f);
        return true;
    } catch (...) {
        return false;
    }
}

void SaveManager::deserializePlayers(Game& game, std::istream& in) {
    // Read each player's state and restore balance, position, status, hand
    for (auto& playerPtr : game.players()) {
        Player& player = *playerPtr;
        std::string username, posCode, statusStr;
        int balance;
        in >> username >> balance >> posCode >> statusStr;
        in.ignore();

        // Restore balance — set directly via operator+= from zero
        int diff = balance - player.balance();
        player += diff;

        // Restore position — find tile index by code
        for (int i = 0; i < game.board().size(); ++i) {
            if (game.board().getTile(i)->code() == posCode) {
                player.setPosition(i);
                break;
            }
        }

        // Restore status
        if (statusStr == "JAILED")        player.setStatus(PlayerStatus::JAILED);
        else if (statusStr == "BANKRUPT") player.setStatus(PlayerStatus::BANKRUPT);
        else                              player.setStatus(PlayerStatus::ACTIVE);

        // Restore hand
        int cardCount; in >> cardCount; in.ignore();
        for (int c = 0; c < cardCount; ++c) {
            std::string line; std::getline(in, line);
            std::istringstream ss(line);
            std::string type; ss >> type;
            int value = 0, dur = 0;
            ss >> value >> dur;

            std::unique_ptr<SkillCard> card;
            if      (type == "MoveCard")       card = std::make_unique<MoveCard>(value);
            else if (type == "DiscountCard")   card = std::make_unique<DiscountCard>(value);
            else if (type == "ShieldCard")     card = std::make_unique<ShieldCard>();
            else if (type == "TeleportCard")   card = std::make_unique<TeleportCard>();
            else if (type == "LassoCard")      card = std::make_unique<LassoCard>();
            else if (type == "DemolitionCard") card = std::make_unique<DemolitionCard>();

            if (card) {
                if (dur > 0) card->setDuration(dur);
                // Inject into skill deck's ownership then hand out the raw pointer
                SkillCard* raw = card.get();
                game.skillDeck().addCard(std::move(card));
                player.addToHand(raw);
            }
        }
    }

    // Restore property ownership based on the already-parsed property state
    // (deserializeProperties runs after this and sets owner pointers by username)
}

void SaveManager::deserializeProperties(Game& game, std::istream& in) {
    int count; in >> count; in.ignore();

    // Build username → Player* lookup
    std::map<std::string, Player*> playerMap;
    for (auto& p : game.players())
        playerMap[p->username()] = p.get();

    for (int i = 0; i < count; ++i) {
        std::string code, jenis, ownerName, status;
        int fmult, fdur; char buildChar;
        in >> code >> jenis >> ownerName >> status >> fmult >> fdur >> buildChar;
        in.ignore();

        Property* prop = game.board().getProperty(code);
        if (!prop) continue;

        // Restore status
        if (status == "OWNED") {
            prop->setStatus(PropertyStatus::OWNED);
            auto it = playerMap.find(ownerName);
            if (it != playerMap.end()) {
                prop->setOwner(it->second);
                it->second->addProperty(prop);
            }
        } else if (status == "MORTGAGED") {
            prop->setStatus(PropertyStatus::MORTGAGED);
            auto it = playerMap.find(ownerName);
            if (it != playerMap.end()) {
                prop->setOwner(it->second);
                it->second->addProperty(prop);
            }
        } else {
            prop->setStatus(PropertyStatus::BANK);
            prop->setOwner(nullptr);
        }

        // Restore building level and festival effect for streets
        if (prop->type() == PropertyType::STREET) {
            auto* s = static_cast<Street*>(prop);
            int lvl = (buildChar == 'H') ? Street::HOTEL : (buildChar - '0');
            s->setBuildingLevel(lvl);
            // Calculate boost count from multiplier (1=0, 2=1, 4=2, 8=3)
            int boosts = 0;
            int m = fmult; while (m > 1) { m >>= 1; ++boosts; }
            s->festival().setState(fmult, fdur, boosts);
        }
    }

    // Recalculate monopoly / railroad / utility counts for all players
    for (auto& p : game.players())
        game.refreshPropertyCounts(p.get());
}

void SaveManager::deserializeDeck(Game& /*game*/, std::istream& in) {
    int count; in >> count;
    in.ignore();
    for (int i = 0; i < count; ++i) {
        std::string type; std::getline(in, type);
        // Rebuild cards into deck — full implementation in milestone 2
    }
}

void SaveManager::deserializeLog(Game& game, std::istream& in) {
    int count; in >> count;
    in.ignore();
    std::vector<LogEntry> entries;
    for (int i = 0; i < count; ++i) {
        LogEntry e;
        std::string line; std::getline(in, line);
        std::istringstream ss(line);
        ss >> e.turn >> e.username >> e.action;
        std::getline(ss, e.detail);
        if (!e.detail.empty() && e.detail[0] == ' ')
            e.detail = e.detail.substr(1);
        entries.push_back(e);
    }
    game.logger().loadEntries(entries);
}

} // namespace Nimonspoli