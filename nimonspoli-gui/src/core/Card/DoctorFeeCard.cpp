#include "DoctorFeeCard.hpp"
#include "../Player/Player.hpp"
#include "../GameState/GameState.hpp"

DoctorFeeCard::DoctorFeeCard()
{
}

DoctorFeeCard::DoctorFeeCard(const string &type, const string &description, int doctorFee) : GeneralFundCard(type, description), doctorFee(doctorFee)
{
}

DoctorFeeCard::~DoctorFeeCard()
{
}

void DoctorFeeCard::execute(Player &p, GameState &gs)
{
    p -= doctorFee;
}