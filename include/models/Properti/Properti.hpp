#pragma once
#include <string>
#include "models/Pemain.hpp"

class Properti{
protected:
    std::string kode;
    std::string nama_properti;
    Pemain* pemilik;
    enum class StatusProperti{
        BANK,
        OWNED,
        MORTGAGED
    };
    StatusProperti status;

public:
    Properti(const std::string& kode, const std::string& nama_properti, Pemain* pemilik, StatusProperti status);

    virtual int hitungSewa(int dadu) = 0;

    const std::string& getKode() const;

    const std::string& getNamaProperti() const;

    StatusProperti getStatus() const;

    Pemain* getPemilik() const;

    void setPemilik(Pemain* pemilik);

    void gadai();

    void batalGadai();

    virtual ~Properti() = default;
};