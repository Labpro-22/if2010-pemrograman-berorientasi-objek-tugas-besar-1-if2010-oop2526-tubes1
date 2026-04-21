#pragma once
#include <string>
using namespace std;
class Tile {
protected:
    int id;
    std::string kode;
    std::string name;
public:
    Tile();
    virtual void onLanded() = 0; 
    virtual ~Tile();
    string getKode();
    string getName();
};