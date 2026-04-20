#include "models/Properti/Properti.hpp"
#include <string>

Properti::Properti(const std::string& kode, const std::string& nama_properti, Pemain* pemilik, StatusProperti status) :
    kode(kode), nama_properti(nama_properti), pemilik(pemilik), status(status){}

const std::string& Properti::getKode() const{
    return kode;
}

const std::string& Properti::getNamaProperti() const{
    return nama_properti;
}

Properti::StatusProperti Properti::getStatus() const{
    return status;
}

Pemain* Properti::getPemilik() const{
    return pemilik;
}

void Properti::setPemilik(Pemain* pemain){
    pemilik = pemain;
    status = StatusProperti::OWNED;
}

void Properti::gadai(){
    status = StatusProperti::MORTGAGED;
}

void Properti::batalGadai(){
    status = StatusProperti::OWNED;
}