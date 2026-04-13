#ifndef SAVELOAD_MANAGER_HPP
#define SAVELOAD_MANAGER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "models/GameState.hpp" // Jordan 
// #include "views/TransactionLogger.hpp" // Kamu yang membuat ini

/**
 * SaveLoadManager bertanggung jawab atas serialisasi dan deserialisasi 
 * seluruh state ke dan dari file .txt. 
 */
class SaveLoadManager {
public:
    // --- Method Utama (Public) ---

    // Menyimpan GameState ke dalam file di folder data/ 
    void save(const std::string& filename, const GameState& gs);

    // Membaca file dari folder data/ dan mengembalikannya dalam bentuk GameState 
    GameState load(const std::string& filename);

    // Memeriksa keberadaan file sebelum operasi muat dilakukan
    bool fileExists(const std::string& filename);

    // Memvalidasi apakah format teks dalam file sesuai dengan spesifikasi 
    bool validateFormat(const std::string& data);

private:
    // --- Method Helper (Private) ---

    // Mengonversi seluruh objek GameMaster/GameState menjadi string besar
    std::string serializeState(const GameState& gs);

    // Mengonversi data properti dari board ke string
    std::string serializeProperties(const Board& board);

    // Mengonversi isi tumpukan kartu ke string
    std::string serializeDeck(const CardDeck& deck);

    // Mengubah string raksasa menjadi objek GameState kembali 
    GameState deserializeState(const std::string& data);
};

#endif