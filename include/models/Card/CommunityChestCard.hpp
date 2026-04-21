#pragma once
#include <string>
using namespace std;

enum class CommunityChestType {
    BIRTHDAY,   
    DOCTOR,     
    CAMPAIGN    
};
 
class CommunityChestCard {
private:
    CommunityChestType type;
    std::string name;
    std::string description;
 
public:
    CommunityChestCard(CommunityChestType type, const std::string& name, const std::string& desc);
 
    CommunityChestCard();
 
    ~CommunityChestCard();
 
    void activate();
 
    CommunityChestType getType() const;
    std::string getName() const;
    std::string getDescription() const;
 
    bool operator==(const CommunityChestCard& other) const;
};
