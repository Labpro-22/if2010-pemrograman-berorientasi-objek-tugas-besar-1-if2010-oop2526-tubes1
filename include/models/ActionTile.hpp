#pragma once

#include "Tile.hpp"



enum ActionType {
    CHANCE,
    COMMUNITY_CHEST,
    FESTIVAL,
    TAX,
    SPECIAL
};

class ActionTile : public Tile {
protected:
    ActionType actionType;

public:
    ActionTile(int position, const std::string& name, const std::string& color,
               ActionType actionType);
    virtual ~ActionTile() = default;

    ActionType getActionType() const;
    void setActionType(ActionType type);

    virtual void executeAction(Player* player) = 0;

    void onLand(Player* player) override;
    void onPass(Player* player) override;
};
