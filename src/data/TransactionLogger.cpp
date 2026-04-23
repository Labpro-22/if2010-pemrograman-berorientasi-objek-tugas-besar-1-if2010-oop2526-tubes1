#include "../../include/data/TransactionLogger.hpp"

#include <sstream>

std::string TransactionLogger::formatEntry(int turn, const std::string& username,
                                           const std::string& actionType,
                                           const std::string& detail) const {
    std::ostringstream oss;
    oss << "[Turn " << turn << "] " << username
        << " | " << actionType << " | " << detail;
    return oss.str();
}

void TransactionLogger::log(int turn, const std::string& username,
                            const std::string& actionType,
                            const std::string& detail) {
    logs.push_back(formatEntry(turn, username, actionType, detail));
}

std::vector<std::string> TransactionLogger::getAll() const {
    return logs;
}

std::vector<std::string> TransactionLogger::getRecent(int count) const {
    std::vector<std::string> out;
    if (count <= 0) return out;
    int total = static_cast<int>(logs.size());
    if (count >= total) return logs;
    for (int i = total - count; i < total; ++i) {
        out.push_back(logs[i]);
    }
    return out;
}

int TransactionLogger::getCount() const {
    return static_cast<int>(logs.size());
}

void TransactionLogger::clear() {
    logs.clear();
}
