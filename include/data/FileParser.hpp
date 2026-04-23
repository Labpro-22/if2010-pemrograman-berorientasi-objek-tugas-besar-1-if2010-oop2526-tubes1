#ifndef FILE_PARSER_HPP
#define FILE_PARSER_HPP

#include <string>
#include <vector>

class FileParser {
protected:
    std::vector<std::string> tokenizeLine(const std::string& line) const;
    std::string trim(const std::string& s) const;
};

#endif
