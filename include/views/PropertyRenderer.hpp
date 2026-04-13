#ifndef PROPERTY_RENDERER_HPP
#define PROPERTY_RENDERER_HPP

#include <string>
#include "models/Property.hpp" // Arin (Person 4)
#include "models/Player.hpp"   // Jordan (Person 1)

class PropertyRenderer {
public:
    // Rendering dasar
    std::string renderDeed(Property& prop);
    std::string renderStreetDeed(StreetProperty& prop);
    std::string renderRailroadDeed(RailroadProperty& prop);
    std::string renderUtilityDeed(UtilityProperty& prop);

    // List properti milik pemain
    std::string renderOwnerProps(Player& player);

    // Helper visual
    std::string renderStatusBadge(std::string status);
    std::string renderFestivalInfo(Property& prop);
    std::string renderBuildingSymbol(int level, bool isHotel);
};

#endif