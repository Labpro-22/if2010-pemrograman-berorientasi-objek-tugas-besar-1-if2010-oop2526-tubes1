#include <vector>
#include <string>
#pragma once

class FileHelper{
public:
    static std::vector<std::string> readAllLines(const std::string& path);
    static void writeAllLines(const std::string& path, const std::vector<std::string>& lines);
};

