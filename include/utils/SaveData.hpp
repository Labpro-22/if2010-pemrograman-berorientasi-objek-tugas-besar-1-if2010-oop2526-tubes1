#pragma once

#include <string>
#include <vector>

class CardSaveData {
public:
    std::string card_type;
    float value;
    int remaining_duration;

    CardSaveData() : value(0), remaining_duration(0) {}
};

class PlayerSaveData {
public:
    std::string username;
    float balance;
    std::string position_code;
    std::string status;
    std::vector<CardSaveData> hand_cards;

    PlayerSaveData() : balance(0) {}
};

class PropertySaveData {
public:
    std::string tile_code;
    std::string type;      
    std::string owner_name; 
    std::string status;    
    int festival_multiplier;
    int festival_duration;
    std::string build_level; 

    PropertySaveData() : festival_multiplier(1), festival_duration(0) {}
};

class LogSaveData {
public:
    int turn;
    std::string username;
    std::string action_type;
    std::string detail;

    LogSaveData() : turn(0) {}
};

class GameSaveData {
public:
    int current_turn;
    int max_turn;
    int player_count;
    
    std::vector<PlayerSaveData> players;
    std::vector<std::string> turn_order;
    std::string current_active_player;
    
    std::vector<PropertySaveData> properties;
    std::vector<std::string> ability_deck; 
    std::vector<LogSaveData> logs;

    GameSaveData() : current_turn(0), max_turn(0), player_count(0) {}
};