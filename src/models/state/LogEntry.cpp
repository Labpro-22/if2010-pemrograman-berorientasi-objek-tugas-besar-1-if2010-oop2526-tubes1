#include "models/state/LogEntry.hpp"

#include <sstream>

LogEntry::LogEntry() : turn(0), username(""), actionType(""), detail("") {}

LogEntry::LogEntry(
    int turn,
    const std::string& username,
    const std::string& actionType,
    const std::string& detail
) : turn(turn), username(username), actionType(actionType), detail(detail) {}

std::string LogEntry::toString() const {
    std::ostringstream oss;
    oss << "[Turn " << turn << "] " << username << " | " << actionType << " | " << detail;
    return oss.str();
}
