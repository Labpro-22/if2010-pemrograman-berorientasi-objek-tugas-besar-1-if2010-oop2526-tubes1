#include <iostream>
#include "utils/ConfigLoader.hpp"

int main() {
    try {
        // Inisialisasi loader dengan path folder config kamu
        ConfigLoader loader("./config"); 

        std::cout << "=== TESTING CONFIG LOADER ===" << std::endl;

        // 1. Test Property
        auto props = loader.loadProperties();
        std::cout << "[OK] Loaded " << props.size() << " properties." << std::endl;
        std::cout << "   Sampel: " << props[0].name << " | Harga: M" << props[0].price << std::endl;

        // 2. Test Railroad Map
        auto rrMap = loader.loadRailroad();
        std::cout << "[OK] Railroad Rent Map size: " << rrMap.size() << std::endl;
        std::cout << "   Sewa 2 stasiun: M" << rrMap[2] << std::endl;

        // 3. Test Tax
        TaxConfig tax = loader.loadTax();
        std::cout << "[OK] Tax Config - PBM Flat: M" << tax.pbmFlat << std::endl;

        // 4. Test Misc
        MiscConfig misc = loader.loadMisc();
        std::cout << "[OK] Misc Config - Max Turn: " << misc.maxTurn << std::endl;

        std::cout << "==============================" << std::endl;
        std::cout << "STATUS: SEMUA DATA TERBACA!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "!!! ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}