#include "utils/SaveLoadManager.hpp"
#include <iostream>

// Operasi File Dasar -

void SaveLoadManager::save(const std::string& filename, const GameState& gs) {
    // Memastikan file disimpan di folder data/ 
    std::ofstream file("data/" + filename);
    if (!file.is_open()) {
        throw std::runtime_error("Gagal menyimpan file! Pastikan folder 'data/' tersedia.");
    }

    file << serializeState(gs);
    file.close();
}

GameState SaveLoadManager::load(const std::string& filename) {
    std::ifstream file("data/" + filename);
    if (!file.is_open()) {
        throw std::runtime_error("File tidak ditemukan!");
    }

    std::string content, line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();

    // Validasi format sebelum di-parse
    if (!validateFormat(content)) {
        throw std::runtime_error("Gagal memuat file! Format tidak dikenali.");
    }

    return deserializeState(content);
}

std::string SaveLoadManager::serializeState(const GameState& gs, const TransactionLogger& logger) {
    std::stringstream ss;

    // 1. Header Global
    ss << gs.getCurrTurn() << " " << gs.getMaxTurn() << "\n";
    
    // 2. Gunakan Helper Functions
    ss << serializePlayers(gs.getTurnOrder());
    
    // 3. Giliran Aktif (Bisa langsung di sini karena simpel)
    ss << gs.getTurnOrder()[gs.getActivePlayerIdx()]->getName() << "\n";

    ss << serializeProperties(*gs.getGameBoard());
    ss << serializeDeck(*gs.getChanceDeck());
    ss << serializeLog(logger);

    return ss.str();
}

std::string SaveLoadManager::serializeProperties(const Board& board) {
    std::stringstream ss;
    auto allTiles = board.getAllTiles();
    
    // Hitung dulu jumlah properti untuk baris pertama <STATE_PROPERTI>
    std::vector<PropertyTile*> props;
    for (auto tile : allTiles) {
        if (tile->isProperty()) {
            props.push_back(static_cast<PropertyTile*>(tile));
        }
    }

    ss << props.size() << "\n";
    for (auto p : props) {
        // Format: <KODE> <JENIS> <PEMILIK> <STATUS> <FMULT> <FDUR> <N_BANGUNAN>
        ss << p->getCardCode() << " " 
           << p->getTypeString() << " " 
           << (p->getOwner() ? p->getOwner()->getName() : "-") << " " 
           << (p->isMortgaged() ? "MORTGAGED" : "OWNED") << " "
           << p->getFestivalMultiplier() << " "
           << p->getFestivalDuration() << " "
           << p->getBuildingLevelString() << "\n";
    }
    return ss.str();
}

// Logika Deserialisasi (Load) 

GameState SaveLoadManager::deserializeState(const std::string& data) {
    std::istringstream stream(data);
    
    int currTurn, maxTurn;
    stream >> currTurn >> maxTurn;

    int numPlayers;
    stream >> numPlayers;

    // Loop untuk membaca data pemain baris demi baris 
    for (int i = 0; i < numPlayers; ++i) {
        std::string uname, pos, status;
        int money, numCards;
        stream >> uname >> money >> pos >> status >> numCards;
        
        // Buat objek Player baru dan masukkan ke vector
    }


    return GameState(maxTurn, ...); 
}

bool SaveLoadManager::validateFormat(const std::string& data) {
    // Placeholder
    return !data.empty(); 
}