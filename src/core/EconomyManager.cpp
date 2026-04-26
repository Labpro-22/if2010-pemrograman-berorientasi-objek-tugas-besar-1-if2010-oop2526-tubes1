#include "../../include/core/EconomyManager.hpp"
#include "../../include/core/TransactionLog.hpp"
#include "../../include/core/GameManager.hpp"
#include <memory>
#include <algorithm>
#include <iostream>
#include "../../include/core/PropertyManager.hpp"
void EconomyManager::addMoney(Player& player, float amount){
    player+=amount;
}
bool EconomyManager::deductMoney(Player& player, float amount){
    if(!player.canPay(amount)){
        return false;
    }
    player-=amount;
    return true;
}
bool EconomyManager::transferMoney(Player& payer, Player& receiver, float amount){
    payer.transferTo(receiver,amount);
    return true;
}
// Pemrosesan pajak
bool EconomyManager::processTax(std::shared_ptr<Player> player, TaxType type, float baseTaxAmount){
    auto &logger=GameManager::logger;
    float tax;
    if (type==TaxType::PPH){
        int pilihan;
        std::cin>>pilihan;
        if( pilihan==1){
            tax=baseTaxAmount;
        }
        else{
            tax=player->getTotalAssetValue() * 0.1;
        }
    }else{
        tax=baseTaxAmount;
    }
    if(deductMoney(*player,tax)){
        logger->recordEvent(LogEntry(0,player->getname(),actions::PAJAK,"bayar pajak sebanyak "));
        return true;
    } else{
        executeBankruptcy(player,nullptr,tax);
        return false;
    }
}
// Proses lelang
void EconomyManager::startAuction(PropertyTile* property){
    active_bidders = GameManager::players;
    current_highest_bid = 0;
    current_highest_bidder.reset();
    size_t bidder_index = 0;
    while (!isAuctionOver()) {
        int bid;
        std::cin >> bid;
        try {
            if (bid == 0) {
                active_bidders.erase(active_bidders.begin() + bidder_index);
                if (bidder_index >= active_bidders.size()) {
                    bidder_index = 0;
                }
            } else {
                placeBid(bid, active_bidders[bidder_index]);
                bidder_index = (bidder_index + 1) % active_bidders.size();
            }
        }
        catch(const char* e) {
            std::cerr << e << " " << current_highest_bid << ">" << bid << '\n';
        }
    }
    resolveAuction(property,active_bidders[0]);
 
}
void EconomyManager::placeBid(float amount, std::shared_ptr<Player> &player){
    if (current_highest_bid<=amount){
        current_highest_bid=amount;
        current_highest_bidder=player;
        return;
    }
    throw "ga cukup uang elu";
}
void EconomyManager::foldBid(std::vector<std::shared_ptr<Player>>::iterator p){
    active_bidders.erase(p);
}
bool EconomyManager::isAuctionOver() const{
    if (active_bidders.size()==1)
    {
        return true;
    }
    return false;
}
void EconomyManager::resolveAuction(PropertyTile *tile, std::shared_ptr<Player> & winner){
    auto& logger= GameManager::logger;
    auto &propMgr=GameManager::property_manager;
    propMgr->assignOwnership(tile,winner);
    logger->recordEvent(LogEntry(0,winner->getname(),actions::LELANG,"si A menang lelang cuy"));
}
std::vector<std::shared_ptr<Player>> EconomyManager::getCurrentBidder() const{
    return this->active_bidders;
}
int EconomyManager::getHighestBid() const{
    return this->current_highest_bid;
}
std::string EconomyManager::toSaveFormat() const {
    return "";
}
// Proses ketika pemain tidak bisa membayar kewajiban
bool EconomyManager::isBankruptcyInevitable(Player& player, float debtAmount) const{
    if(player.getTotalAssetValue()<debtAmount){
        return true;
    }
    return false;
}
void EconomyManager::executeBankruptcy(std::shared_ptr<Player>  bankruptPlayer, std::shared_ptr<Player> creditor,float amount){
    if(isBankruptcyInevitable(*bankruptPlayer,amount)){
        bankruptPlayer->declareBankruptcy();
    }
    // printopsi(bankruptPlayer)
    PropertyTile *dijual = nullptr;
    //dijual=pilihopsi(bankruptPlayer)
    if (dijual != nullptr) // jual=true, mortgage=false;
    {
        bankruptPlayer->sellProperty(*dijual);
    } else{
        GameManager::property_manager.get()->tryMortgage(bankruptPlayer,dijual);
    }
    if (creditor) {
        bankruptPlayer->transferTo(*creditor,amount);
    }
}


