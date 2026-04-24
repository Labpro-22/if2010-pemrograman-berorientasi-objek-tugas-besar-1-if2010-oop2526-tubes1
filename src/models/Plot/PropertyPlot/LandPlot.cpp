#include "models/Plot/PropertyPlot/LandPlot.hpp"

LandPlot::LandPlot(std::string name, std::string code, Color color,
            int mortgageValue, int buyPrice, int upgHousePrice, int upgHotelPrice,
            std::map<int, int> rentPriceTable, Player* owner, PropertyStatus propertyStatus,
            int festivalDuration, int festivalMultiplier):
    PropertyPlot(name, code, color, buyPrice, mortgageValue, owner, propertyStatus, festivalDuration, festivalMultiplier),
    upgHousePrice(upgHousePrice), upgHotelPrice(upgHotelPrice),
    rentPriceTable(rentPriceTable){}

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

void LandPlot::build(PlotContext& ctx){
    canBuild(ctx);
    
    if (level == 4){
        owner->pay(upgHotelPrice);
    }
    else{
        owner->pay(upgHousePrice);
    }
}

void LandPlot::sellBuildings(){
    if (level <= 0){
        throw BuildingIsEmptyException();
    }

    level--;
    //tambah uang tergantung level
    if (level == 5){
        owner->receive(upgHotelPrice);
    }
    else{
        owner->receive(upgHousePrice);
    }
}

bool LandPlot::canBuild(PlotContext& ctx) const {
    int buildCost = getBuildCost();
    if (!isStreetOwned(ctx, &ctx.getCurrentPlayer())){
        throw ColorSetNotOwnedException();
    }
    if (owner->getCash() < getBuildCost()){
        throw InsufficientFundException();
    }
    if (level >= 5){
        throw BuildingIsFullException();


    }
    return true;
} 

int LandPlot::getBuildCost() const {
    if (level < 4){
        return upgHousePrice;
    }
    else if (level == 4){
        return upgHotelPrice;
    } else {
        return 0;
    }
}

bool LandPlot::isStreetOwned(PlotContext& ctx, Player* player) const {
    if (owner == NULL) return false;
    return ctx.getBoard().isPlayerOwnAllColor(color, player);
}

int LandPlot::calculateRentPrice(PlotContext& ctx) const {
    int rentPrice = rentPriceTable.at(level);

    if (level == 0 && isStreetOwned(ctx, owner)){
        rentPrice *= 2;
    }

    return rentPrice*festivalMultiplier;
}

PlotType LandPlot::getType() const {
    return PlotType::LANDPLOT;
}

void LandPlot::startEvent(PlotContext& ctx){
    if (!isOwned()){
        if (ctx.getCurrentPlayer().getCash() >= getBuyPrice()
            && CommandHandler::promptYesNo("Apakah anda ingin membeli petak ini (harga: M" + std::to_string(getBuyPrice()) + ")?")){
            ctx.getCurrentPlayer().buyProperty(*this);
        }
        else{
            // ctx.getAuctionService().startAuction(); //TODO: perbaiki auction service
        }
    }
    else{
        if (owner != &ctx.getCurrentPlayer()){
            int rentPrice = calculateRentPrice(ctx);
            ctx.getCurrentPlayer().pay(rentPrice); //TODO: handle bankrupt
            owner->receive(rentPrice);
        }
    }
}