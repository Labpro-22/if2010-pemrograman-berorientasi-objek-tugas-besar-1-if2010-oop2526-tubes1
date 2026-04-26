#include "../include/utils/IOManager.hpp"
#include "../include/utils/ConfigData.hpp"
#include "../include/core/GameManager.hpp"
#include <memory>
#include <vector>
// untuk test doang 
int main(){
    std::vector<std::shared_ptr<Player>> players;
    players.push_back(std::make_shared<Player>("Pemain1", 1500, 0, PlayerState::FREE));
    players.push_back(std::make_shared<Player>("Pemain2", 1500, 0, PlayerState::FREE));
        players.push_back(std::make_shared<Player>("Pemain3", 1500, 0, PlayerState::FREE));
    players[0]->addSkillCard(std::make_unique<MoveSkillCard>("move"));
        players[0]->addSkillCard(std::make_unique<DiscountSkillCard>("discount"));
    players[0]->addSkillCard(std::make_unique<ShieldSkillCard>("shield"));
    players[0]->addSkillCard(std::make_unique<TeleportSkillCard>("teleport"));
        players[0]->addSkillCard(std::make_unique<LassoSkillCard>("lasso"));



    auto game = std::make_unique<GameManager>(
        100,
        std::move(players),
        std::make_unique<CardManager>(),
        std::make_unique<PropertyManager>(nullptr),
        std::make_unique<EconomyManager>(),
        std::make_unique<TransactionLog>()
    );

    game->startGame();
    return 0;
}