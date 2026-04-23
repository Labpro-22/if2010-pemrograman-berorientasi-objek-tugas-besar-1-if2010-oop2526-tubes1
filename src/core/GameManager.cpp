#include "GameManager.hpp"
#include <iostream>





void GameManager::visitCardTile(CardTile* tile, Player& player) {
    if(tile->getType() == CHANCE){
        card_manager->drawKesempatan(player);
    }
    else{
        card_manager->drawDanaUmum(player);
    }
}

void GameManager::visitTaxTile(TaxTile* tile, Player& player) {
    economy_manager->deductMoney(player, tile->getTaxAmount());
}

void GameManager::visitFestivalTile(FestivalTile* tile, Player& player) {
    //idk right now
}

void GameManager::visitGoTile(GoTile* tile, Player& player) {
    economy_manager->addMoney(player, tile->getReward());
}

void GameManager::visitGoToJailTile(GoToJailTile* tile, Player& player) {
    player.setInJail();
}

void GameManager::visitFreeParkingTile(FreeParkingTile* tile, Player& player) {
    //do nothing
}

void GameManager::visitJailTile(JailTile* tile, Player& player) {
    //do nothing
}

void GameManager::visitStreetTile(StreetTile* tile, Player& player) {
    std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
    int rent = tile->calculateRent();
    if(current_owner && current_owner.get() != &player){
        bool success = this->economy_manager->transferMoney(player, *current_owner, rent);
    }
}

void GameManager::visitRailroadTile(RailroadTile* tile, Player& player) {
    std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
    int rent = tile->calculateRent();
    if(current_owner && current_owner.get() != &player){
        bool success = this->economy_manager->transferMoney(player, *current_owner, rent);
    }
}


void GameManager::visitUtilityTile(UtilityTile* tile, Player& player) {
    std::shared_ptr<Player> current_owner = tile->getPropertyOwner().lock();
    int rent = tile->calculateRent();
    if(current_owner && current_owner.get() != &player){
        bool success = this->economy_manager->transferMoney(player, *current_owner, rent);
    }
}
