#include "../../include/models/LogEntry.hpp"
#include <sstream>

LogEntry::LogEntry(int turn, const string& username, const string& action,
                   const string& detail) : turn_(turn), username_(username), action_(action), detail_(detail) {}

int LogEntry::getTurn() const { return turn_; }
string LogEntry::getUsername() const { return username_; }
string LogEntry::getAction() const { return action_; }
string LogEntry::getDetail() const { return detail_; }

string LogEntry::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turn_ << "] "
        << username_ << " | "
        << action_   << " | "
        << detail_;
    return oss.str();
}