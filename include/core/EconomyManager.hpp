#pragma once

#include <memory>
#include <vector>
#include "../../include/models/Player.hpp"
#include "../../include/models/PropertyTile.hpp"
#include "../../include/models/Saveable.hpp"

class TransactionLog; 

class EconomyManager : Saveable {
private:
    std::vector<std::shared_ptr<Player>> active_bidders;
    int current_highest_bid;
    std::shared_ptr<Player> current_highest_bidder;
    int bidder_index;
    PropertyTile* auctioned_property;

public:
    EconomyManager() = default;
    ~EconomyManager() = default;

    // Transaksi Dasar
    void addMoney(Player& player, float amount);
    bool deductMoney(Player& player, float amount);
    bool transferMoney(Player& payer, Player& receiver, int amount);

    // Pemrosesan pajak
    bool processTax(Player& player, TaxType type, int baseTaxAmount);

    // Proses lelang
    void startAuction(PropertyTile* property, const std::vector<std::shared_ptr<Player>>& players);
    void placeBid(int amount, std::shared_ptr<Player> &player);
    void EconomyManager::foldBid(std::vector<std::shared_ptr<Player>>::iterator p);
    bool isAuctionOver() const;
    void resolveAuction(PropertyTile *tile,std::shared_ptr<Player> & winner);
    std::vector<std::shared_ptr<Player>> getCurrentBidder() const;
    int getHighestBid() const;

    // Proses ketika pemain tidak bisa membayar kewajiban

    bool isBankruptcyInevitable(Player& player, int debtAmount) const;
    void executeBankruptcy(std::shared_ptr<Player>  bankruptPlayer, std::shared_ptr<Player> creditor,int amount);
};