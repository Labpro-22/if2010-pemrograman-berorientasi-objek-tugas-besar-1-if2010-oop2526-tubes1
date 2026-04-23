#pragma once

#include <string>
#include <vector>
#include <map>
#include "models/Plot/Colors.hpp"
#include "core/PlotContext.hpp"

class Plot{
protected:
    std::string name;
    std::string code;

public:
    Plot(std::string name, std::string code);
    virtual ~Plot() = default;

    std::string getName() const;
    std::string getCode() const;

    virtual std::string getType() const = 0;
    virtual Color getColor() const;
    virtual void startEvent(PlotContext& context) = 0;  //TODO: add parameter
    //TODO: updateState method??
};