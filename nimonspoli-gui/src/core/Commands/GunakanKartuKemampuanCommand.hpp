#ifndef GUNAKAN_KARTU_KEMAMPUAN_COMMAND_HPP
#define GUNAKAN_KARTU_KEMAMPUAN_COMMAND_HPP

#include "Command.hpp"

class GunakanKemampuanCommand : public Command
{
public:
    GunakanKemampuanCommand() = default;
    void execute(GameMaster &gm) override;
};

#endif