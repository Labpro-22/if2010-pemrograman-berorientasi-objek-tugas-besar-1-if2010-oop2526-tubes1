#include "RailroadTile.hpp"
#include "BankruptcyController.hpp"
RailroadTile::RailroadTile(int idx, string code, string name, 
    int price, int morgageValue, vector<int> rentPrices) : 
    PropertyTile(idx, code, name, price, morgageValue, rentPrices) {
    this->PropertyType = RAILROAD;
}

void RailroadTile::land(Player &p) {
    if (!isOwned()) {
        setOwner(&p);
        setStatus(OWNED);
    } else if (owner != &p && status == OWNED) {
        Player *ownerPlayer = owner;
        int count = ownerPlayer->countOwnerRailroads();
        int rent = rentPrices[count - 1]; 
        if (!BankruptcyController::checkSolvency(p, rent)) {
            BankruptcyController::declareBankruptcy(p, *ownerPlayer);
            return;
        } 
        p -= rent;
        *ownerPlayer += rent;
    }
}
