#include "../../include/utils/IOManager.hpp"
#include <algorithm>

FullConfigData IOManager::loadAllConfigs(const std::string& configDirectoryPath){
    IOManager manager;
    std::vector<std::unique_ptr<Tile>> allTiles;
    std::vector<std::unique_ptr<Tile>> Properties = manager.IOparsePropertyTileConfig(configDirectoryPath+"/property.txt");
    std::vector<int> taxes = manager.IOparseTaxConfig(configDirectoryPath+"/tax.txt");
    std::vector<int> special = manager.IOparseSpecialConfig(configDirectoryPath+"/special.txt");
    std::vector<std::unique_ptr<Tile>> ActionTile = manager.IOparseActionTileConfig(configDirectoryPath+"/aksi.txt",taxes,special);
    std::vector<int> misc = manager.IOparseMiscConfig(configDirectoryPath+"/misc.txt");
    std::map<int,int> RailroadSewa = manager.IOparseRailroadConfig(configDirectoryPath+"/railroad.txt");
    std::map<int,int> UtilityMult = manager.IOparseUtilityConfig(configDirectoryPath+"/utility.txt");

    return FullConfigData(std::move(Properties),std::move(ActionTile),RailroadSewa,UtilityMult,taxes.at(0),taxes.at(1),taxes.at(2),special.at(0),special.at(1),misc.at(0),misc.at(1));

    // std::cout << "GO_SALARY: " << std::to_string(special.at(0)) << std::endl;
    // std::cout << "JAIL_FINE: " << std::to_string(special.at(1)) << std::endl <<std::endl;
    // std::cout << "PPH_FLAT: " << std::to_string(taxes.at(0)) << std::endl;
    // std::cout << "PPH_PERCENT: " << std::to_string(taxes.at(1)) << std::endl;
    // std::cout << "PBM_FLAT: " << std::to_string(taxes.at(2)) << std::endl << std::endl;
    // std::cout << "MAX_TURN: " << std::to_string(special.at(0)) << std::endl;
    // std::cout << "SALDO_AWAL: " << std::to_string(special.at(1)) << std::endl <<std::endl;
    
    // for(auto& tile : Properties) {
    //     allTiles.push_back(std::move(tile));
    // }
    // for(auto& tile : ActionTile) {
    //     allTiles.push_back(std::move(tile));
    // }
    // std::sort(allTiles.begin(), allTiles.end(), 
    //     [](const std::unique_ptr<Tile>& a, const std::unique_ptr<Tile>& b) {
    //         return a->getIndex() < b->getIndex();
    //     });
    // for (const auto& tile : allTiles){
    //     std::cout << tile->getIndex() << " " << tile->getCode() << " " << tile->getName() << std::endl;
    // }
    // std::cout << "\n";
    // for(const auto& pair : RailroadSewa){
    //     std::cout << "Jumlah Railroad: " << pair.first << " (" << pair.second << ")" << std::endl;
    // }
    // for(const auto& pair : UtilityMult){
    //     std::cout << "Jumlah Utility: " << pair.first << " (" << pair.second << ")" << std::endl;
    // }
}

std::vector<std::unique_ptr<Tile>> IOManager::IOparsePropertyTileConfig(const std::string& filepath){
    std::ifstream file(filepath);
    std::string line;

    int ID, HARGA_LAHAN, NILAI_GADAI, UPG_RUMAH, UPG_HT, RENT_L0, RENT_L1, RENT_L2, RENT_L3, RENT_L4, RENT_L5;
    std::string KODE, NAMA, JENIS, WARNA;
    std::vector<std::unique_ptr<Tile>> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> ID >> KODE >> NAMA >> JENIS){
            if(JENIS=="STREET"){
                if(iss >> WARNA >> HARGA_LAHAN >> NILAI_GADAI >> UPG_RUMAH >> UPG_HT >> RENT_L0 >> RENT_L1 >> RENT_L2 >> RENT_L3 >> RENT_L4 >> RENT_L5){
                    std::vector<int> rent;
                    rent.push_back(RENT_L0);
                    rent.push_back(RENT_L1);
                    rent.push_back(RENT_L2);
                    rent.push_back(RENT_L3);
                    rent.push_back(RENT_L4);
                    rent.push_back(RENT_L5);
                    temp.push_back(std::make_unique<StreetTile>(ID, NAMA, KODE, WARNA, HARGA_LAHAN, NILAI_GADAI, nullptr, 0, 0, BANK, 0, rent, UPG_RUMAH, UPG_HT));
                }
            }
            else if(JENIS=="RAILROAD"){
                if(iss >> WARNA >> HARGA_LAHAN >> NILAI_GADAI){
                    temp.push_back(std::make_unique<RailroadTile>(ID, NAMA, KODE, WARNA, HARGA_LAHAN, NILAI_GADAI, nullptr, 0, 0, BANK));
                }
            }
            else if(JENIS=="UTILITY"){
                if(iss >> WARNA >> HARGA_LAHAN >> NILAI_GADAI){
                    temp.push_back(std::make_unique<UtilityTile>(ID, NAMA, KODE, WARNA, HARGA_LAHAN, NILAI_GADAI, nullptr, 0, 0, BANK));
                }
            }
        }
    }
    return temp;
}
std::vector<int> IOManager::IOparseTaxConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int PPHFLAT, PPHPERCENT, PBM;
    std::vector<int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> PPHFLAT >> PPHPERCENT >> PBM){
            temp.push_back(PPHFLAT);
            temp.push_back(PPHPERCENT);
            temp.push_back(PBM);
        }
    }
    return temp;
}
std::vector<int> IOManager::IOparseSpecialConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int GO, JAIL;
    std::vector<int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> GO >> JAIL){
            temp.push_back(GO);
            temp.push_back(JAIL);
        }
    }
    return temp;
}
std::vector<std::unique_ptr<Tile>> IOManager::IOparseActionTileConfig(const std::string& configDirectoryPath, std::vector<int>& tax, std::vector<int>& special){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int ID;
    std::string KODE, NAMA, JENIS, WARNA;
    std::vector<std::unique_ptr<Tile>> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> ID >> KODE >> NAMA >> JENIS >> WARNA){
            if(JENIS=="SPESIAL"){
                if(NAMA=="Petak_Mulai"){
                    temp.push_back(std::make_unique<GoTile>(ID, NAMA, KODE, WARNA, special.at(0)));
                }
                else if(NAMA=="Penjara"){
                    temp.push_back(std::make_unique<JailTile>(ID, NAMA, KODE, WARNA, special.at(1)));
                }
                else if(NAMA=="Petak_Pergi_ke_Penjara"){
                    temp.push_back(std::make_unique<GoToJailTile>(ID, NAMA, KODE, WARNA));
                }
                else if(NAMA=="Bebas_Parkir"){
                    temp.push_back(std::make_unique<FreeParkingTile>(ID, NAMA, KODE, WARNA));
                }
            }
            else if(JENIS=="KARTU"){
                if(NAMA=="Kesempatan"){
                    temp.push_back(std::make_unique<CardTile>(ID, NAMA, KODE, WARNA, CHANCE));
                }
                else if(NAMA=="Dana_Umum"){
                    temp.push_back(std::make_unique<CardTile>(ID, NAMA, KODE, WARNA, COMMUNITY_CHEST));
                }
            }
            else if(JENIS=="PAJAK"){
                if(NAMA=="Pajak_Penghasilan"){
                    temp.push_back(std::make_unique<TaxTile>(ID, NAMA, KODE, WARNA, PPH, tax.at(0)));
                }
                else if(NAMA=="Pajak_Barang_Mewah"){
                    temp.push_back(std::make_unique<TaxTile>(ID, NAMA, KODE, WARNA, PBM, tax.at(2)));
                }
            }
            else if(JENIS=="FESTIVAL"){
                temp.push_back(std::make_unique<FestivalTile>(ID, NAMA, KODE, WARNA));
            }
        }
    }
    return temp;
}

std::vector<int> IOManager::IOparseMiscConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int MAX_TURN, SALDO_AWAL;
    std::vector<int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> MAX_TURN >> SALDO_AWAL){
            temp.push_back(MAX_TURN);
            temp.push_back(SALDO_AWAL);
        }
    }
    return temp;
}
std::map<int,int> IOManager::IOparseRailroadConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int JUMLAH_RAILROAD, BIAYA_SEWA;
    std::map<int,int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> JUMLAH_RAILROAD >> BIAYA_SEWA){
            temp.insert({JUMLAH_RAILROAD,BIAYA_SEWA});
        }
    }
    return temp;
}
std::map<int,int> IOManager::IOparseUtilityConfig(const std::string& configDirectoryPath){
    std::ifstream file(configDirectoryPath);
    std::string line;

    int JUMLAH_UTILITY, FAKTOR_PENGALI;
    std::map<int,int> temp;

    while(std::getline(file,line)){
        std::istringstream iss(line);
        if(iss >> JUMLAH_UTILITY >> FAKTOR_PENGALI){
            temp.insert({JUMLAH_UTILITY,FAKTOR_PENGALI});
        }
    }
    return temp;
}