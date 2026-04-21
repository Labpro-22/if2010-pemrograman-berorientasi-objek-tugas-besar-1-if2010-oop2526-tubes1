#include "models/Papan.hpp"

Papan::Papan(){
// Membuat 40 petak papan, diambil dari ManagerProperti
    // std::vector<PetakInit> initData = {
    //     {1, "GO", "Petak Mulai", "SPECIAL"},
    //     {2, "GRT", "Garut", "PROPERTY"},
    //     {3, "DNU", "Dana Umum", "CARD"},
    //     {4, "TSK", "Tasikmalaya", "PROPERTY"},
    //     {5, "PPH", "Pajak Penghasilan", "TAX"},
    //     {6, "GBR", "Stasiun Gambir", "RAILROAD"},
    //     // dst...
    // };

    // for(const auto& data : initData){

    //     if(data.tipe == "PetakProperti"){
    //         daftarPetak.push_back(
    //             new PetakProperti(
    //                 data.indeks,
    //                 data.kode,
    //                 data.nama,
    //                 managerProperti.getProperti(data.kode)
    //             )
    //         );
    //     }

    //     else if(data.tipe == "PetakStasiun"){
    //         daftarPetak.push_back(
    //             new PetakProperti(
    //                 data.indeks,
    //                 data.kode,
    //                 data.nama,
    //                 managerProperti.getProperti(data.kode)
    //             )
    //         );
    //     }

    //     else if(data.tipe == "PetakUtility"){
    //         daftarPetak.push_back(
    //             new PetakProperti(
    //                 data.indeks,
    //                 data.kode,
    //                 data.nama,
    //                 managerProperti.getProperti(data.kode)
    //             )
    //         );
    //     }

    //     else if(data.tipe == "PetakKartu"){
    //         // nanti pakai PetakKartu
    //     }

    //     else if(data.tipe == "PetakPajak"){
    //         // nanti pakai PetakPajak
    //     }

    //     else if(data.tipe == "PetakSpesial"){
    //         // nanti pakai PetakSpesial turunan
    //     }

    //     else if(data.tipe == "PetakFestival"){
    //         // nanti pakai PetakFestival
    //     }
    // }
}


Petak* Papan::getPetak(int indeks){
    if(indeks >=0 && indeks < getTotalPetak()){
        for(Petak* petak : daftarPetak){
            if(petak->getIndeks() == indeks){
                return petak;
            }
        }
        // Petak tidak ditemukan
        return nullptr;
    }
    // indeks tidak valid
    return nullptr;
}

Petak* Papan::getPetak(const std::string& kode){
    for(Petak* petak : daftarPetak){
        if(petak->getKode() == kode){
            return petak;
        }
    }
    return nullptr;
}

// Mencari petak dengan kode tertentu yang terdekat dari pemain pada indeks tertentu
Petak* Papan::findNearestByKode(const std::string& kode, int indeks){
    for(int i = 0; i < getTotalPetak(); i++){
        int idx = (indeks + i) % 40;

        if(daftarPetak.at(idx)->getKode() == kode){
            return daftarPetak.at(idx);
        }
    }

    // Kalau tidak ditemukan sama sekali : error / exception mungkin
    return nullptr;
}

int Papan::getTotalPetak(){
    return daftarPetak.size();
}