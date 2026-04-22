#pragma once
#include "ActionTile.hpp"

class GameContext;

enum SpecialType { 
    GO, 
    JAIL, 
    FREE_PARKING, 
    GO_TO_JAIL 
};

class SpecialTile : public ActionTile {
protected:
    SpecialType specialType;

public:
    SpecialTile(int position, const std::string& name, const std::string& color,
                SpecialType specialType);
    virtual ~SpecialTile() = default;

    SpecialType getSpecialType() const;
    void setSpecialType(SpecialType type);

    virtual void executeSpecial(Player* player, GameContext* ctx) = 0;
    void executeAction(Player* player, GameContext* ctx) override;
    virtual void onPass(Player* player) override;

};