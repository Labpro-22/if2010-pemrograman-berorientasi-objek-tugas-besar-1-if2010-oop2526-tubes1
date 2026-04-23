#pragma once
#include <string>
#include <vector>
#include <algorithm>

enum actions{
    DADU, // roll dadu 
    DOUBLE, // kalau roll dice dapet double, ex. 6 and 6 
    SEWA, // current player bayar sewa ke player lain 
    BELI, // beli tile
    RAILROAD, // dapet railroad
    UTILITY, // dapet utility 
    KARTU, // use a skill card
    FESTIVAL, // land on a festival tile
    GOJAIL, // land on gotojail tile 
    LELANG, // bid on an auction
    GADAI,  // gadai property 
    PAJAK,      // land on bayar pajak tile
    UNMORTGAGE,     // pay back the money u owe and get back ur property
    BANKRUPT,   // poor, L + ratio + maidenless
    KESEMPATAN,     // land di kesempatan
    DANA_UMUM,  // land di dana umum
    WIN,    // ada player yang memebuhi win con
    BUILD_HOUSE, // build a house
    BUILD_HOTEL, // build a hotel
    SAVE, 
    LOAD
};
extern const char* actionStrings[];

class LogEntry{
private:
    int turn_number;
    std::string username;
    actions action_type;
    std::string description;

public:
    LogEntry(int turns, std::string username, actions action_type, std::string desc)
    : turn_number(turns), username(username), action_type(action_type), description(desc) {}

    void setDesc(std::string desc);
    int getTurnNumber()const;
    std::string getUsername() const;
    std::string getDesc() const;
    actions getActionType() const;
    std::string printFormat()const;
    const char * getTextForEnum( int enumVal )const
    {
      return actionStrings[enumVal];
    }

    ~LogEntry(){}
};

class TransactionLog{
private:
    std::vector<LogEntry> logs;
public:
    TransactionLog(){   }
    void recordEvent(LogEntry entry);
    std::string getAllLogs()const;
    std::string getRecentLogs(int n)const;
    void clearLogs();
    std::string toSave();

    ~TransactionLog(){}
};