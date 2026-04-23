#include "../../include/views/PropertyView.hpp"
#include "../../include/models/Property.hpp"
#include "../../include/models/Player.hpp"

#include <iostream>

using namespace std;

void PropertyView::showPropertyDetail(Property* property) {
    if (!property) return;

    cout << "Property: " << property->getName() << endl;
    cout << "Owner: " << property->getOwner() << endl;
}

void PropertyView::showPlayerProperties(Player* player) {
    if (!player) return;

    cout << "Properties milik: " << player->getUsername() << endl;
    // nanti isi dari GameController
}