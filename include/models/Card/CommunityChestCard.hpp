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
    string name;
    string description;
 
public:
    CommunityChestCard(CommunityChestType type, const string& name, const string& desc);
 
    CommunityChestCard();
 
    ~CommunityChestCard();
 
    void activate();
 
    CommunityChestType getType() const;
    string getName() const;
    string getDescription() const;
 
    bool operator==(const CommunityChestCard& other) const;
};
