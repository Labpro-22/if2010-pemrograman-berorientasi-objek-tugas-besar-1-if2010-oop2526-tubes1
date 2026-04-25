#include "../../include/core/EconomyManager.hpp"
#include "../../include/core/TransactionLog.hpp"
#include "../../include/core/GameManager.hpp"
#include <memory>
#include <algorithm>
#include <iostream>
#include <PropertyManager.hpp>
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
bool EconomyManager::transferMoney(Player& payer, Player& receiver, int amount){
    payer.transferTo(receiver,amount);
}
// Pemrosesan pajak
bool EconomyManager::processTax(Player& player, TaxType type, int baseTaxAmount){
    auto &logger=GameManager::logger;
    float tax;
    if (type==TaxType::PPH){
        int pilihan;
        std::cin>>pilihan;
        if( pilihan==1){
            tax=baseTaxAmount;
        }
        else{
            tax=player.getTotalAssetValue() * 0.1;
        }
    }else{
        tax=baseTaxAmount;
    }
    if(deductMoney(player,tax)){
        logger->recordEvent(LogEntry(0,player.getname(),actions::PAJAK,"bayar pajak sebanyak "));
    } else{
        executeBankruptcy(player,nullptr);
    }
}
// Proses lelang
void EconomyManager::startAuction(PropertyTile* property, const std::vector<std::shared_ptr<Player>>& players){
    for (auto& p:players){
        active_bidders.push_back(p);
    }
    auto p=active_bidders.begin();
    while (!isAuctionOver())
    {
        int bid;
        std::cin>>bid;
        try{
            if (bid==0){
                foldBid(p);
                p++;
                if (p == active_bidders.end()) {
                    p = active_bidders.begin();
                }
            } else{
                placeBid(bid,*p);
            }  
            p++;
            if (p == active_bidders.end()) {
                p = active_bidders.begin();
            } 
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
        }
    }
 
}
void EconomyManager::placeBid(int amount, std::shared_ptr<Player> &player){
    if (current_highest_bid<=amount){
        current_highest_bid=amount;
        current_highest_bidder=player;
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
// Proses ketika pemain tidak bisa membayar kewajiban
bool EconomyManager::isBankruptcyInevitable(Player& player, int debtAmount) const{
    if(player.getTotalAssetValue()<debtAmount){
        return true;
    }
    return false;
}
void EconomyManager::executeBankruptcy(std::shared_ptr<Player>  bankruptPlayer, std::shared_ptr<Player> creditor,int amount){
    if(isBankruptcyInevitable(*bankruptPlayer,amount)){
        bankruptPlayer->declareBankruptcy();
    }
    // printopsi(bankruptPlayer)
    PropertyTile &dijual;
    //dijual=pilihopsi(bankruptPlayer)
    if (condition) //jual=true, mortgage=false;
    {
        bankruptPlayer->sellProperty(dijual);
    } else{
        GameManager::property_manager.get()->tryMortgage(bankruptPlayer,&dijual);
    }
    bankruptPlayer->transferTo(*creditor,amount);
}
