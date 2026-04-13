#include "views/BoardRenderer.hpp"
#include <vector>
#include <string>
#include <iostream>

int main() {
    BoardRenderer renderer;
    
    // 1. Siapkan 32 kode petak dummy (Sesuai ID 0-31)
    std::vector<std::string> codes;
    for(int i=0; i<32; i++) {
        // Kita kasih nama contoh: JKT, BDG, dsb.
        if (i == 0) codes.push_back("START");
        else if (i == 8) codes.push_back("PARK");
        else if (i == 16) codes.push_back("FES");
        else if (i == 24) codes.push_back("JAIL");
        else {
            codes.push_back("P" + std::to_string(i));
        }
    }

    // 2. Bersihkan layar terminal sebelum print
    // Gunakan ANSI escape code agar lebih pro daripada system("clear")
    std::cout << "\033[2J\033[H"; 

    std::cout << "--- TESTING DISPLAY NIMONSPOLI MILSTONE 1 ---" << std::endl;
    
    // 3. Jalankan Render (Turn 15 dari 50)
    renderer.renderBoard(codes, 15);

    std::cout << "\n[INFO] Jika petak kanan nempel di ujung, berarti simetris!" << std::endl;
    std::cout << "[INFO] Gunakan terminal yang lebar agar tidak patah." << std::endl;

    return 0;
}