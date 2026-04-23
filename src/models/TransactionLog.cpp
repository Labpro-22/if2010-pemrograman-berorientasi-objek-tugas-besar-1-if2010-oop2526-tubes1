#include "../../include/models/TransactionLog.hpp"


const char* actionStrings[] = {
    "DADU", 
    "DOUBLE", 
    "SEWA", 
    "BELI", 
    "RAILROAD", 
    "UTILITY", 
    "KARTU", 
    "FESTIVAL", 
    "GOJAIL", 
    "LELANG", 
    "GADAI",  
    "PAJAK",      
    "UNMORTGAGE",     
    "BANKRUPT",   
    "KESEMPATAN",     
    "DANA_UMUM",  
    "WIN",    
    "BUILD_HOUSE", 
    "BUILD_HOTEL", 
    "SAVE", 
    "LOAD"
};

void LogEntry::setDesc(std::string desc){
    description=desc;
}
int LogEntry::getTurnNumber()const{
    return turn_number;
}
std::string LogEntry::getUsername()const{
    return username;
}
std::string LogEntry::getDesc()const{
    return description;
}
actions LogEntry::getActionType()const{
    return action_type;
}
std::string LogEntry::printFormat()const{
    std::string temp;
    std::string enumstr=getTextForEnum(getActionType());
    for(int i=0;i<11-enumstr.length();i++){
        temp+=" ";
    }
    return "[Turn "+std::to_string(getTurnNumber())+"] "+getUsername()+" | "+enumstr+temp+" | "+getDesc();
}

void TransactionLog::recordEvent(LogEntry entry){
    logs.push_back(entry);
}
std::string TransactionLog::getAllLogs()const{
    if(logs.empty()) return "";
    std::string temp;
    for(auto it=logs.begin();it<logs.end();it++){
        temp+=it->printFormat();
        if(it+1!=logs.end()){
            temp+="\n";
        }
    }
    return temp;
}
std::string TransactionLog::getRecentLogs(int n)const{
    if(n<=0) return "";
    std::string temp;
    int size = logs.size();
    int start_idx = std::max(0, size - n);
    for(int i=start_idx;i<size;i++){
        temp+=logs.at(i).printFormat();
        if(i+1!=logs.size()){
            temp+="\n";
        }
    }
    return temp;
}
void TransactionLog::clearLogs(){
    logs.clear();
}
std::string TransactionLog::toSave(){
    if(logs.empty()) return "";
    std::string temp;
    for(LogEntry log : logs){
        temp+=(std::to_string(log.getTurnNumber())+log.getUsername()+log.getTextForEnum(log.getActionType())+log.getDesc());
    }
    return temp;
}