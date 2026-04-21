#include "models/Card/CommunityChestCard.hpp"
#include <iostream>
 
CommunityChestCard::CommunityChestCard(CommunityChestType type, const string& name, const string& desc)
    : type(type), name(name), description(desc) {}
 
CommunityChestCard::CommunityChestCard(): type(CommunityChestType::DOCTOR), name(""), description("") {}
 
CommunityChestCard::~CommunityChestCard() {}
 
void CommunityChestCard::activate() {
    cout << "Kartu: \"" << description << "\"" << endl;
}
 
CommunityChestType CommunityChestCard::getType() const {
    return type;
}
 
string CommunityChestCard::getName() const {
    return name;
}
 
string CommunityChestCard::getDescription() const {
    return description;
}
 
bool CommunityChestCard::operator==(const CommunityChestCard& other) const {
    return type == other.type;
}