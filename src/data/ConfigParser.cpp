#include "../../include/data/ConfigParser.hpp"

#include "../../include/utils/FileFormatException.hpp"
#include "../../include/utils/CardDeck.hpp"
#include "../../include/utils/ActionCard.hpp"

#include "../../include/models/GameBoard.hpp"
#include "../../include/models/Tile.hpp"
#include "../../include/models/Street.hpp"
#include "../../include/models/Railroad.hpp"
#include "../../include/models/Utility.hpp"
#include "../../include/models/GoTile.hpp"
#include "../../include/models/JailTile.hpp"
#include "../../include/models/FreeParkingTile.hpp"
#include "../../include/models/GoToJailTile.hpp"
#include "../../include/models/ChanceTile.hpp"
#include "../../include/models/CommunityChestTile.hpp"
#include "../../include/models/TaxTile.hpp"
#include "../../include/models/FestivalTile.hpp"

#include <fstream>
#include <cstdlib>
#include <cctype>

namespace {

int parseInt(const std::string& s, const std::string& context) {
    if (s.empty()) {
        throw FileFormatException("Empty integer in " + context);
    }
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (i == 0 && (c == '-' || c == '+')) continue;
        if (c < '0' || c > '9') {
            throw FileFormatException("Non-integer token '" + s + "' in " + context);
        }
    }
    return std::atoi(s.c_str());
}

std::string toUpper(const std::string& s) {
    std::string out = s;
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(out[i])));
    }
    return out;
}

bool startsWithDigit(const std::string& s) {
    return !s.empty() && s[0] >= '0' && s[0] <= '9';
}

}

ConfigParser::ConfigParser(const std::string& basePath)
    : basePath(basePath),
      pphFlat(0), pphPercentage(0), pbmFlat(0),
      goSalary(0), jailFine(0),
      maxTurn(0), startBalance(0),
      chanceDeck(nullptr), communityDeck(nullptr) {
    chanceDeck = new CardDeck<ActionCard>();
    communityDeck = new CardDeck<ActionCard>();
}

ConfigParser::~ConfigParser() {
    delete chanceDeck;
    delete communityDeck;
}

void ConfigParser::parsePropertyConfig(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        throw FileFormatException("Cannot open " + filename);
    }
    std::string line;
    bool headerConsumed = false;
    while (std::getline(in, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;
        std::vector<std::string> tokens = tokenizeLine(trimmed);
        if (tokens.empty()) continue;
        if (!headerConsumed) {
            headerConsumed = true;
            if (!startsWithDigit(tokens[0])) continue;
        }
        if (tokens.size() < 5) {
            throw FileFormatException("property.txt: too few fields on line: " + trimmed);
        }

        int id = parseInt(tokens[0], "property.txt ID");
        int position = id - 1;
        std::string code = tokens[1];
        std::string name = tokens[2];
        std::string type = toUpper(tokens[3]);
        std::string colorRaw = tokens[4];

        Tile* tile = nullptr;
        if (type == "STREET") {
            if (tokens.size() < 10) {
                throw FileFormatException("property.txt: street needs at least 10 fields");
            }
            int harga = parseInt(tokens[5], "property.txt HARGA");
            int gadai = parseInt(tokens[6], "property.txt NILAI_GADAI");
            int upgRumah = parseInt(tokens[7], "property.txt UPG_RUMAH");
            int upgHotel = parseInt(tokens[8], "property.txt UPG_HT");

            std::string up = toUpper(colorRaw);
            ColorGroup colorGroup;
            if (up == "COKLAT" || up == "COKELAT") colorGroup = COKELAT;
            else if (up == "BIRU_MUDA") colorGroup = BIRU_MUDA;
            else if (up == "MERAH_MUDA" || up == "PINK") colorGroup = PINK;
            else if (up == "ORANGE") colorGroup = ORANGE;
            else if (up == "MERAH") colorGroup = MERAH;
            else if (up == "KUNING") colorGroup = KUNING;
            else if (up == "HIJAU") colorGroup = HIJAU;
            else if (up == "BIRU_TUA") colorGroup = BIRU_TUA;
            else throw FileFormatException("Unknown color group: " + colorRaw);

            Street* s = new Street(position, name, colorRaw, colorGroup,
                                   harga, gadai, upgRumah, upgHotel);
            std::map<int, int> rentTable;
            int levels = static_cast<int>(tokens.size()) - 9;
            if (levels < 1) {
                throw FileFormatException("property.txt: street missing rent levels");
            }
            for (int lvl = 0; lvl < levels; ++lvl) {
                rentTable[lvl] = parseInt(tokens[9 + lvl], "property.txt rent level");
            }
            s->setRentTable(rentTable);
            tile = s;
        } else if (type == "RAILROAD") {
            if (tokens.size() < 7) {
                throw FileFormatException("property.txt: railroad needs 7 fields");
            }
            int harga = parseInt(tokens[5], "property.txt railroad HARGA");
            int gadai = parseInt(tokens[6], "property.txt railroad NILAI_GADAI");
            tile = new Railroad(position, name, colorRaw, harga, gadai);
        } else if (type == "UTILITY") {
            if (tokens.size() < 7) {
                throw FileFormatException("property.txt: utility needs 7 fields");
            }
            int harga = parseInt(tokens[5], "property.txt utility HARGA");
            int gadai = parseInt(tokens[6], "property.txt utility NILAI_GADAI");
            tile = new Utility(position, name, colorRaw, harga, gadai);
        } else {
            throw FileFormatException("property.txt: unknown property type " + type);
        }
        if (tile != nullptr) tile->setCode(code);
        stagedTiles[position] = tile;
    }
}

void ConfigParser::parseRailroadConfig(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        throw FileFormatException("Cannot open " + filename);
    }
    railroadRentTable.clear();
    std::string line;
    bool headerConsumed = false;
    while (std::getline(in, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;
        std::vector<std::string> tokens = tokenizeLine(trimmed);
        if (tokens.size() < 2) continue;
        if (!headerConsumed) {
            headerConsumed = true;
            if (!startsWithDigit(tokens[0])) continue;
        }
        int count = parseInt(tokens[0], "railroad.txt JUMLAH");
        int rent = parseInt(tokens[1], "railroad.txt BIAYA_SEWA");
        railroadRentTable[count] = rent;
    }
}

void ConfigParser::parseUtilityConfig(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        throw FileFormatException("Cannot open " + filename);
    }
    utilityMultiplierTable.clear();
    std::string line;
    bool headerConsumed = false;
    while (std::getline(in, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;
        std::vector<std::string> tokens = tokenizeLine(trimmed);
        if (tokens.size() < 2) continue;
        if (!headerConsumed) {
            headerConsumed = true;
            if (!startsWithDigit(tokens[0])) continue;
        }
        int count = parseInt(tokens[0], "utility.txt JUMLAH");
        int mult = parseInt(tokens[1], "utility.txt FAKTOR");
        utilityMultiplierTable[count] = mult;
    }
}

void ConfigParser::parseTaxConfig(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        throw FileFormatException("Cannot open " + filename);
    }
    std::string line;
    bool headerConsumed = false;
    while (std::getline(in, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;
        std::vector<std::string> tokens = tokenizeLine(trimmed);
        if (tokens.size() < 3) continue;
        if (!headerConsumed) {
            headerConsumed = true;
            if (!startsWithDigit(tokens[0])) continue;
        }
        pphFlat = parseInt(tokens[0], "tax.txt PPH_FLAT");
        pphPercentage = parseInt(tokens[1], "tax.txt PPH_PERSENTASE");
        pbmFlat = parseInt(tokens[2], "tax.txt PBM_FLAT");
        return;
    }
}

void ConfigParser::parseSpecialConfig(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        throw FileFormatException("Cannot open " + filename);
    }
    std::string line;
    bool headerConsumed = false;
    while (std::getline(in, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;
        std::vector<std::string> tokens = tokenizeLine(trimmed);
        if (tokens.size() < 2) continue;
        if (!headerConsumed) {
            headerConsumed = true;
            if (!startsWithDigit(tokens[0])) continue;
        }
        goSalary = parseInt(tokens[0], "special.txt GO_SALARY");
        jailFine = parseInt(tokens[1], "special.txt JAIL_FINE");
        return;
    }
}

void ConfigParser::parseMiscConfig(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        throw FileFormatException("Cannot open " + filename);
    }
    std::string line;
    bool headerConsumed = false;
    while (std::getline(in, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;
        std::vector<std::string> tokens = tokenizeLine(trimmed);
        if (tokens.size() < 2) continue;
        if (!headerConsumed) {
            headerConsumed = true;
            if (!startsWithDigit(tokens[0])) continue;
        }
        maxTurn = parseInt(tokens[0], "misc.txt MAX_TURN");
        startBalance = parseInt(tokens[1], "misc.txt SALDO_AWAL");
        return;
    }
}

void ConfigParser::parseActionConfig(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        throw FileFormatException("Cannot open " + filename);
    }
    std::string line;
    bool headerConsumed = false;
    while (std::getline(in, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty()) continue;
        std::vector<std::string> tokens = tokenizeLine(trimmed);
        if (tokens.empty()) continue;
        if (!headerConsumed) {
            headerConsumed = true;
            if (!startsWithDigit(tokens[0])) continue;
        }
        if (tokens.size() < 5) {
            throw FileFormatException("aksi.txt: too few fields on line: " + trimmed);
        }

        int id = parseInt(tokens[0], "aksi.txt ID");
        int position = id - 1;
        std::string code = tokens[1];
        std::string name = tokens[2];
        std::string jenis = toUpper(tokens[3]);
        std::string color = tokens[4];
        std::string upCode = toUpper(code);

        Tile* tile = nullptr;
        if (jenis == "SPESIAL" || jenis == "SPECIAL") {
            if (upCode == "GO") {
                tile = new GoTile(position, name, color, goSalary);
            } else if (upCode == "PEN" || upCode == "JAIL" || upCode == "PJR") {
                tile = new JailTile(position, name, color, jailFine);
            } else if (upCode == "BBP" || upCode == "FP") {
                tile = new FreeParkingTile(position, name, color);
            } else if (upCode == "PPJ" || upCode == "GTJ") {
                tile = new GoToJailTile(position, name, color);
            } else {
                tile = new FreeParkingTile(position, name, color);
            }
        } else if (jenis == "KARTU" || jenis == "CARD") {
            if (upCode == "DNU" || upCode == "DU") {
                tile = new CommunityChestTile(position, name, color, communityDeck);
            } else {
                tile = new ChanceTile(position, name, color, chanceDeck);
            }
        } else if (jenis == "PAJAK" || jenis == "TAX") {
            if (upCode == "PBM") {
                tile = new TaxTile(position, name, color, PBM, pbmFlat, 0.0);
            } else {
                tile = new TaxTile(position, name, color, PPH, pphFlat,
                                   static_cast<double>(pphPercentage) / 100.0);
            }
        } else if (jenis == "FESTIVAL") {
            tile = new FestivalTile(position, name, color);
        } else {
            throw FileFormatException("aksi.txt: unknown JENIS_PETAK " + jenis);
        }
        if (tile != nullptr) tile->setCode(code);
        stagedTiles[position] = tile;
    }
}

void ConfigParser::loadConfig(GameBoard* board) {
    if (board == nullptr) {
        throw FileFormatException("ConfigParser::loadConfig: board is null");
    }
    const std::string sep = "/";
    parseTaxConfig(basePath + sep + "tax.txt");
    parseSpecialConfig(basePath + sep + "special.txt");
    parseMiscConfig(basePath + sep + "misc.txt");
    parseRailroadConfig(basePath + sep + "railroad.txt");
    parseUtilityConfig(basePath + sep + "utility.txt");
    parsePropertyConfig(basePath + sep + "property.txt");
    parseActionConfig(basePath + sep + "aksi.txt");

    if (stagedTiles.empty()) {
        throw FileFormatException("No tiles loaded from config");
    }
    int maxPos = stagedTiles.rbegin()->first;
    for (int i = 0; i <= maxPos; ++i) {
        std::map<int, Tile*>::iterator it = stagedTiles.find(i);
        if (it == stagedTiles.end()) {
            throw FileFormatException("Missing tile at position " + std::to_string(i));
        }
        board->addTile(it->second);
    }
    stagedTiles.clear();

    const std::vector<Tile*>& tiles = board->getTiles();
    for (size_t i = 0; i < tiles.size(); ++i) {
        Railroad* r = dynamic_cast<Railroad*>(tiles[i]);
        if (r != nullptr) {
            r->setRentTable(railroadRentTable);
            continue;
        }
        Utility* u = dynamic_cast<Utility*>(tiles[i]);
        if (u != nullptr) {
            u->setMultiplierTable(utilityMultiplierTable);
        }
    }
}
