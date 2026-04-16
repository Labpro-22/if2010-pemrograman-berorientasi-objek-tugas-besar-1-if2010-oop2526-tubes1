#include "TransactionLog.hpp"

void LogEntry::setDesc(std::string desc){
    description=desc;
}
int LogEntry::getTurnNumber(){
    return turn_number;
}
std::string LogEntry::getUsername(){
    return username;
}
std::string LogEntry::getDesc(){
    return description;
}

void TransactionLog::recordEvent(LogEntry entry){
    logs.push_back(entry);
}
const std::vector<LogEntry>& TransactionLog::getAllLogs() const{
    return logs;
}
std::vector<LogEntry> TransactionLog::getRecentLogs(int n) const {
    if(n<=0) return std::vector<LogEntry>();
    int size = logs.size();
    int start_idx = std::max(0, size - n);
    return std::vector<LogEntry>(logs.begin() + start_idx, logs.end());
}
void TransactionLog::clearLogs(){
    logs.clear();
}