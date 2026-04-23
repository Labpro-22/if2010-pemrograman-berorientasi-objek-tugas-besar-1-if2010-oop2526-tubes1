#include "./FestivalCommand.hpp"
#include <iostream>

FestivalCommand::FestivalCommand(Player *p, TransactionLogger *logger, int turnNumber)
    : p(p), logger(logger), turnNumber(turnNumber) {}

void FestivalCommand::execute(GameMaster&)
{
    std::cout << "\nKamu mendarat di petak Festival!\n";

    std::vector<StreetProperty *> streets;
    for (Property *prop : p->getProperties())
    {
        StreetProperty *sp = dynamic_cast<StreetProperty *>(prop);
        if (sp && sp->getStatus() != PropertyStatus::MORTGAGED)
            streets.push_back(sp);
    }

    if (streets.empty())
    {
        std::cout << "Kamu belum memiliki properti lahan. Efek festival tidak berlaku.\n";
        return;
    }

    StreetProperty *selected = promptSelectProperty();
    if (!selected)
        return;

    applyFestival(selected);
}

StreetProperty *FestivalCommand::promptSelectProperty()
{
    // Tampilkan semua StreetProperty milik player
    std::vector<StreetProperty *> streets;
    for (Property *prop : p->getProperties())
    {
        StreetProperty *sp = dynamic_cast<StreetProperty *>(prop);
        if (sp && sp->getStatus() != PropertyStatus::MORTGAGED)
            streets.push_back(sp);
    }

    std::cout << "Daftar properti milikmu:\n";
    for (auto *sp : streets)
    {
        std::cout << "  - " << sp->getCode()
                  << " (" << sp->getName() << ")"
                  << "  [sewa saat ini: M"
                  << sp->calculateRentPrice(0, 1, false) << "]\n";
    }
    std::cout << "Masukkan kode properti (0 untuk batal): ";

    std::string code;
    std::cin >> code;
    if (code == "0")
        return nullptr;

    for (auto *sp : streets)
    {
        if (sp->getCode() == code)
            return sp;
    }

    // Cek apakah kode itu milik player tapi bukan Street / sedang digadai
    for (Property *prop : p->getProperties())
    {
        if (prop->getCode() == code)
        {
            std::cout << "-> Properti ini tidak memenuhi syarat festival (bukan lahan atau sedang digadai)!\n";
            return nullptr;
        }
    }

    std::cout << "-> Kode properti tidak valid atau bukan milikmu!\n";
    return nullptr;
}

void FestivalCommand::applyFestival(StreetProperty *prop)
{
    int curMult = prop->getFestivalMultiplier();
    int rentBefore = prop->calculateRentPrice(0, 1, false); 

    if (curMult < 8)
    {
        int newMult = (curMult == 1) ? 2 : curMult * 2;
        prop->activateFestival(newMult); 

        int rentAfter = prop->calculateRentPrice(0, 1, false);

        if (curMult == 1)
        {
            std::cout << "Efek festival aktif!\n";
        }
        else
        {
            std::cout << "Efek diperkuat!\n";
        }
        std::cout << "Sewa sebelumnya : M" << rentBefore << "\n";
        std::cout << "Sewa sekarang   : M" << rentAfter << "\n";
        std::cout << "Durasi          : " << prop->getFestivalDuration() << " giliran\n";

        logger->addLog(turnNumber, p->getUsername(), "FESTIVAL",
                       prop->getCode() + ": sewa M" + std::to_string(rentBefore) +
                           " -> M" + std::to_string(rentAfter) +
                           ", durasi " + std::to_string(prop->getFestivalDuration()) + " giliran");
    }
    else
    {
        prop->setFestivalDuration(3);
        std::cout << "Efek sudah maksimum (harga sewa sudah digandakan tiga kali).\n";
        std::cout << "Sewa tetap      : M" << rentBefore << "\n";
        std::cout << "Durasi di-reset menjadi: 3 giliran\n";

        logger->addLog(turnNumber, p->getUsername(), "FESTIVAL",
                       prop->getCode() + ": efek maks, durasi di-reset 3 giliran");
    }
}