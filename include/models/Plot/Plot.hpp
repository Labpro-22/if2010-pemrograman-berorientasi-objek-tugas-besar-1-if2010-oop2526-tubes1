#pragma once

#include <string>
#include <vector>
#include <map>
#include "models/Plot/Colors.hpp"
#include "models/Player/Player.hpp"
#include "core/PlotContext.hpp"
#include "core/GameException.hpp"

class Plot{
protected:
    std::string name;
    std::string code;
    Color color;

public:
    Plot(std::string name, std::string code, Color color);
    virtual ~Plot() = default;

    std::string getName() const;
    std::string getCode() const;
    Color getColor() const;

    virtual std::string getType() const = 0;
    virtual Player* getOwner() const;
    virtual void startEvent(PlotContext& ctx) = 0;
};