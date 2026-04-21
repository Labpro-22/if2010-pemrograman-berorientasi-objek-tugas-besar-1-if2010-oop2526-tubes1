#ifndef PROPERTY_VIEW_HPP
#define PROPERTY_VIEW_HPP

class Property;
class Player;

class PropertyView {
public:
    void showPropertyDetail(Property* property);
    void showPlayerProperties(Player* player);
};

#endif