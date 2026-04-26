#include "CetakPropertiCommand.hpp"
#include "../GameMaster/GameMaster.hpp"
#include "../GameState/GameState.hpp"
#include "../Player/Player.hpp"

#include <iostream>
#include <string>

using namespace std;

string CetakPropertiCommand::getOutput(GameMaster &gm)
{
    Player *player = gm.getState().getCurrPlayer();

    if (player == nullptr)
    {
        return "Tidak ada pemain aktif.\n";
    }

    string hasil = player->cetakProperti();

    if (hasil.empty())
    {
        return "Kamu belum memiliki properti apapun.\n";
    }

    return hasil;
}

void CetakPropertiCommand::execute(GameMaster &gm)
{
    cout << getOutput(gm);
}