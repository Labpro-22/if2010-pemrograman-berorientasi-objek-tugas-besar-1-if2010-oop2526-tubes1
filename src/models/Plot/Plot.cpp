#include "models/Plot/Plot.hpp"

Plot::Plot(std::string name, std::string code): name(name), code(code) {}

std::string Plot::getName() const {
    return name;
}

std::string Plot::getCode() const {
    return code;
}

Color Plot::getColor() const {
    return Color::DEFAULT;
}