#pragma once
#include <iostream>
#include <string>
using namespace std;

class LogEntry
{
private:
    int turn;
    string username;
    string actionType;
    string detail;
public:
    LogEntry();
    LogEntry(int turn, string username, string actionType, string detail);
    int getTurn() const;
    string getUsername() const;
    string getActionType() const;
    string getDetail() const;
    string toString() const;
};