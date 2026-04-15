#pragma once
#include <string>
using namespace std;

class LogEntry {
private:
    int turn_;
    string username;
    string action_;
    string detail_;
public: 
    LogEntry(int turn, const string& username, 
             const string& action, const string& detail);
    
    int getTurn() const;
    string getUsername() const;
    string getAction() const;
    string getDetail() const;
    string toString() const;
};