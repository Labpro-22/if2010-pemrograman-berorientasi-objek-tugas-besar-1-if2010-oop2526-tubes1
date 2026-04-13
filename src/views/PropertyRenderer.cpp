#include "views/PropertyRenderer.hpp"

std::string PropertyRenderer::renderBuildingSymbol(int level, bool isHotel) {
    if (isHotel) return "[ H ]"; // Hotel
    if (level == 0) return "[---]";
    std::string res = "[";
    for(int i=0; i<level; i++) res += "*"; // Simbol Rumah
    for(int i=0; i<4-level; i++) res += "-";
    return res + "]";
}

std::string PropertyRenderer::renderStatusBadge(std::string status) {
    if (status == "MORTGAGED") return " < TERGADAI > ";
    return " < MILIK PRIBADI > ";
}