#include "models/Plot/PropertyPlot/LandPlot.hpp"

LandPlot::LandPlot(std::string name, std::string code, Color color,
            int mortgageValue, int buyPrice, int upgHousePrice, int upgHotelPrice,
            std::map<int, int> rentPriceTable, PropertyStatus propertyStatus = PropertyStatus::BANK):
    PropertyPlot(name, code, color, mortgageValue, propertyStatus),
    upgHousePrice(upgHousePrice), upgHotelPrice(upgHotelPrice),
    rentPriceTable(rentPriceTable) {}

int LandPlot::getBuyPrice() const {
    return buyPrice;
}

int LandPlot::getUpgHousePrice() const {
    return upgHotelPrice;
}

int LandPlot::getUpgHotelPrice() const {
    return upgHotelPrice;
}

std::map<int, int> LandPlot::getRentPriceTable() const {
    return rentPriceTable;
}

int LandPlot::getRentPrice(int level) const {
    return rentPriceTable.at(level);
}

int LandPlot::getLevel() const {
    return level;
}

void LandPlot::build(){
    if (!canBuild) return;

    if (level == 4){
        //money -= upgHotelPrice;
    }
    else{
        //money -= upgHousePrice;
    }
}

void LandPlot::sellBuildings(){ //TODO:
    if (level <= 0){
        //throw exception level kurang
    }

    level--;
    //tambah uang tergantung level
    if (level == 5){
        //tambah 
    }
    else{

    }
}

bool LandPlot::canBuild() const { //TODO:
    //TODO: check uang
    //if uang cukup &&
    if (level >= 5){
        //std::cout << "Tidak bisa menambah bangunan. << std::endl;
        //std::cout << "Tanah sudah mencapai level maximum" << std::endl;
        // atau throw exception
    }
} 

int LandPlot::calculateRentPrice() const {
    int rentPrice = rentPriceTable.at(level);

    if (level == 0){
        //TODO: return 2 kali harga dasar jika street ownedBy Player
    }

    return rentPrice*festivalMultiplier;
}

std::string LandPlot::getType() const {
    return "Petak Lahan";
}

void LandPlot::startEvent(PlotContext& ctx) {

}