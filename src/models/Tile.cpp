#include "../../include/models/Tile.hpp"

Tile:: Tile(int id, string kode, string name) : id(id), kode(kode), name(name){}

int Tile:: getIndex(){
    return this->id;
}

string Tile:: getKode(){
    return this->kode;
}

string Tile:: getName(){
    return this->name;
}