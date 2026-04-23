#pragma once

#include <string>
#include <vector>

struct LogEntry {
    int turn;
    std::string username;
    std::string actionType;
    std::string detail;
};

class Logger {
private:
    std::vector<LogEntry> entries;

public:
    void log(const LogEntry& entry);
    std::vector<LogEntry> getRecent(int n) const;
    std::vector<LogEntry> getAll() const;
};
