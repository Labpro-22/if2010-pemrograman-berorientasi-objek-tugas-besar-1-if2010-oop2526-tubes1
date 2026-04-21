#include "../../include/views/PlayerView.hpp"
// #include "../../include/models/Player.hpp"

#include <iostream>

using namespace std;

void PlayerView::showPlayerStatus(Player* player) {
    if (!player) return;

    cout << "Player: " << player->getUsername() << endl;
    cout << "Money: " << player->getMoney() << endl;
    cout << "Position: " << player->getPosition() << endl;
}