#include <iostream>
#include <vector>
#include "views/TransactionLogger.hpp"

int main() {
    TransactionLogger logger;

    std::cout << "--- Menambah Log Transaksi ---" << std::endl;
    logger.addLog(1, "Andra", "START", "Mendapatkan uang awal 1000");
    logger.addLog(1, "Andra", "ROLL", "Mendarat di Jakarta (JKT)");
    logger.addLog(1, "Andra", "BUY", "Membeli Jakarta seharga 350");
    logger.addLog(2, "Jordan", "CARD", "Mendapatkan kartu Shield");
    
    // Tes 1: Ambil semua log untuk keperluan Serialisasi (Save)
    std::cout << "\nTes 1: Semua Log (untuk Save)" << std::endl;
    const auto& allLogs = logger.getLogs();
    std::cout << "Jumlah Log: " << allLogs.size() << std::endl;
    for (const auto& log : allLogs) {
        std::cout << "[" << log.turn << "] " << log.username << ": " << log.detail << std::endl;
    }

    // Tes 2: Ambil 2 log terbaru saja (untuk Panel UI)
    std::cout << "\nTes 2: 2 Log Terbaru (untuk UI)" << std::endl;
    std::vector<LogEntry> lastTwo = logger.getLastLogs(2);
    for (const auto& log : lastTwo) {
        std::cout << "> " << log.detail << std::endl;
    }

    // Tes 3: Clear Log
    std::cout << "\nTes 3: Menghapus Log" << std::endl;
    logger.clear();
    std::cout << "Jumlah Log setelah clear: " << logger.getLogs().size() << std::endl;

    return 0;
}