#include "models/Plot/Plot.hpp"

Plot::Plot(std::string name, std::string code, Color color): name(name), code(code), color(color) {}

std::string Plot::getName() const {
    return name;
}

std::string Plot::getCode() const {
    return code;
}

Color Plot::getColor() const {
    return color;
}

Player* Plot::getOwner() const {
    return NULL;
}