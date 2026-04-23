#include "utils/Logger.hpp"

#include <algorithm>
#include <stdexcept>

void Logger::log(const LogEntry& entry) {
    entries.push_back(entry);
}

std::vector<LogEntry> Logger::getRecent(int n) const {
    if (n < 0) {
        throw std::invalid_argument("n tidak boleh negatif.");
    }
    if (n == 0 || entries.empty()) {
        return {};
    }

    const std::size_t count = std::min(static_cast<std::size_t>(n), entries.size());
    const auto start = entries.end() - static_cast<std::vector<LogEntry>::difference_type>(count);
    return std::vector<LogEntry>(start, entries.end());
}

std::vector<LogEntry> Logger::getAll() const {
    return entries;
}
