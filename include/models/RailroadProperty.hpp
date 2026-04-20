#pragma once

#include "Property.hpp"
#include <map>

class GameContext;

class RailroadProperty : public Property {
private:
    map<int,int> rentByCount; // map jumlah railroad dimiliki → sewa
public:
    RailroadProperty(const string& code, const string& name, int purchasePrice, int mortgageValue,
                     const map<int,int>& rentByCount);
    int  calculateRent(const GameContext& ctx) const override;
    bool canBuild() const override;
    int  getSellValue() const override;
    const map<int,int>& getRentByCount() const;
};