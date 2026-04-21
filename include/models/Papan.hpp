#pragma once
#include "models/Petak/Petak.hpp"
#include "models/PlayerActionService.hpp"

#include <vector>

class Papan{
private:
    std::vector<Petak*> daftarPetak;

public:
    Papan(); // Butuh ManagerProperti

    Petak* getPetak(int indeks);

    Petak* getPetak(const std::string& kode);

    Petak* findNearestByKode(const std::string& kode, int indeks);

    int getTotalPetak();

    ~Papan() = default;
};