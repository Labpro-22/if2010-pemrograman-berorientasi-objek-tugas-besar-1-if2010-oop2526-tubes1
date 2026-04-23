#pragma once

#include <memory>
#include <vector>
#include "Player.hpp"
#include "PropertyTile.hpp"
#include "Saveable.hpp"

class PropertyManager;
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
    void addMoney(Player& player, int amount);
    bool deductMoney(Player& player, int amount);
    bool transferMoney(Player& payer, Player& receiver, int amount);

    // Pemrosesan pajak
    bool processTax(Player& player, TaxType type, int baseTaxAmount, TransactionLog& logger);

    // Proses lelang
    void startAuction(PropertyTile* property, const std::vector<std::shared_ptr<Player>>& players);
    bool placeBid(int amount);
    void foldBid();
    bool isAuctionOver() const;
    void resolveAuction(PropertyManager& propMgr, TransactionLog& logger);
    std::shared_ptr<Player> getCurrentBidder() const;
    int getHighestBid() const;

    // Proses ketika pemain tidak bisa membayar kewajiban

    bool isBankruptcyInevitable(const Player& player, int debtAmount) const;
    void executeBankruptcy(Player& bankruptPlayer, 
                           std::shared_ptr<Player> creditor, 
                           PropertyManager& propMgr, 
                           TransactionLog& logger);
};