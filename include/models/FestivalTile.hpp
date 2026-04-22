#pragma once
#include "ActionTile.hpp"

//Forward Declaration 
class Property;
class GameContext;

class FestivalTile : public ActionTile {
public:
    FestivalTile(int position, const std::string& name, const std::string& color);
    ~FestivalTile() = default;

    void applyFestivalEffect(Player* player, Property* property);
    void updateDuration();
    void executeAction(Player* player, GameContext* ctx) override;
};