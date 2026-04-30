#pragma once

#include <vector>

class TradeToPlayer;
class Player;
class PropertyTile;

class TradeManager {
private:
    std::vector<TradeToPlayer*> activeTrades;

public:
    TradeManager() = default;
    ~TradeManager();

    TradeToPlayer* proposeTrade(Player* proposer, Player* target,
                                const std::vector<PropertyTile*>& offeredProps, int offeredMoney,
                                const std::vector<PropertyTile*>& requestedProps, int requestedMoney);

    bool validateTrade(TradeToPlayer* trade) const;

    void acceptTrade(TradeToPlayer* trade);

    void rejectTrade(TradeToPlayer* trade);
    
    std::vector<TradeToPlayer*> getTradesForPlayer(Player* target) const;

    const std::vector<TradeToPlayer*>& getActiveTrades() const;

    void clearActiveTrades();

    void restoreTrade(Player* proposer, Player* target,
                    const std::vector<PropertyTile*>& offeredProps, int offeredMoney,
                    const std::vector<PropertyTile*>& requestedProps, int requestedMoney);

};