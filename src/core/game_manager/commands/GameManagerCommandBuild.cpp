#include "core/game_manager/GameManager.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../GameManagerInternal.hpp"
#include "models/base/Property.hpp"
#include "models/tiles/Street.hpp"

void GameManager::commandBuild(const std::string &propertyCode)
{
    Player &player = getCurrentPlayer();
    std::string code = GameManagerInternal::toUpper(propertyCode);

    if (code.empty() && display != 0)
    {
        std::vector<std::string> groups;
        std::ostringstream oss;
        oss << "=== Color Group yang Memenuhi Syarat ===\n";

        const std::vector<Property *> &owned = player.getOwnedProperties();
        for (std::size_t i = 0; i < owned.size(); ++i)
        {
            Street *ownedStreet = dynamic_cast<Street *>(owned[i]);
            if (ownedStreet == 0 || !ownedStreet->isMonopoly())
            {
                continue;
            }

            const std::string group = ownedStreet->getColorGroup();
            if (std::find(groups.begin(), groups.end(), group) != groups.end())
            {
                continue;
            }

            std::vector<Property *> groupProperties = board.getPropertiesByColorGroup(group);
            bool anyBuildable = false;
            for (std::size_t j = 0; j < groupProperties.size(); ++j)
            {
                Street *groupStreet = dynamic_cast<Street *>(groupProperties[j]);
                if (groupStreet != 0 && groupStreet->getOwner() == &player && groupStreet->canBuild())
                {
                    anyBuildable = true;
                }
            }
            if (!anyBuildable)
            {
                continue;
            }

            groups.push_back(group);
            oss << groups.size() << ". [" << GameManagerInternal::colorGroupDisplay(group) << "]\n";
            for (std::size_t j = 0; j < groupProperties.size(); ++j)
            {
                Street *groupStreet = dynamic_cast<Street *>(groupProperties[j]);
                if (groupStreet != 0 && groupStreet->getOwner() == &player)
                {
                    const int nextCost = groupStreet->getBuildingLevel() >= 4 ? groupStreet->getHotelCost() : groupStreet->getHouseCost();
                    oss << "   - " << GameManagerInternal::prettyName(groupStreet->getName())
                        << " (" << groupStreet->getCode() << ")"
                        << " : " << GameManagerInternal::buildingLevelLabel(groupStreet->getBuildingLevel())
                        << " (Harga " << (groupStreet->getBuildingLevel() >= 4 ? "hotel" : "rumah")
                        << ": " << GameManagerInternal::formatMoney(nextCost) << ")\n";
                }
            }
        }

        if (groups.empty())
        {
            display->printMessage("Tidak ada color group yang memenuhi syarat untuk dibangun.");
            display->printMessage("Kamu harus memiliki seluruh petak dalam satu color group terlebih dahulu.");
            return;
        }

        display->printMessage(oss.str());
        display->printMessage("Uang kamu saat ini : " + GameManagerInternal::formatMoney(player.getMoney()));
        int groupChoice = std::atoi(display->getInput("Pilih nomor color group (0 untuk batal): ").c_str());
        if (groupChoice <= 0 || static_cast<std::size_t>(groupChoice) > groups.size())
        {
            return;
        }

        const std::string selectedGroup = groups[static_cast<std::size_t>(groupChoice - 1)];
        std::vector<Property *> groupProperties = board.getPropertiesByColorGroup(selectedGroup);
        int minLevel = 5;
        for (std::size_t i = 0; i < groupProperties.size(); ++i)
        {
            Street *groupStreet = dynamic_cast<Street *>(groupProperties[i]);
            if (groupStreet != 0)
            {
                minLevel = std::min(minLevel, groupStreet->getBuildingLevel());
            }
        }

        std::vector<Street *> options;
        std::ostringstream groupOss;
        groupOss << "Color group [" << GameManagerInternal::colorGroupDisplay(selectedGroup) << "]:\n";
        for (std::size_t i = 0; i < groupProperties.size(); ++i)
        {
            Street *groupStreet = dynamic_cast<Street *>(groupProperties[i]);
            if (groupStreet == 0 || groupStreet->getOwner() != &player)
            {
                continue;
            }

            options.push_back(groupStreet);
            groupOss << options.size() << ". "
                     << GameManagerInternal::prettyName(groupStreet->getName())
                     << " (" << groupStreet->getCode() << ")"
                     << " : " << GameManagerInternal::buildingLevelLabel(groupStreet->getBuildingLevel());
            if (groupStreet->getBuildingLevel() >= 5)
            {
                groupOss << " <- sudah maksimal, tidak dapat dibangun";
            }
            else if (groupStreet->getBuildingLevel() > minLevel)
            {
                groupOss << " <- belum memenuhi pemerataan";
            }
            else if (groupStreet->getBuildingLevel() >= 4)
            {
                groupOss << " <- siap upgrade ke hotel";
            }
            else
            {
                groupOss << " <- dapat dibangun";
            }
            groupOss << '\n';
        }

        display->printMessage(groupOss.str());
        int propertyChoice = std::atoi(display->getInput("Pilih petak (0 untuk batal): ").c_str());
        if (propertyChoice <= 0 || static_cast<std::size_t>(propertyChoice) > options.size())
        {
            return;
        }
        code = options[static_cast<std::size_t>(propertyChoice - 1)]->getCode();
    }

    Street *street = dynamic_cast<Street *>(findPropertyOrThrow(code));
    if (street == 0)
    {
        throw std::runtime_error("Only streets can be built.");
    }
    if (street->getOwner() != &player)
    {
        throw std::runtime_error("Street not owned by current player.");
    }
    if (!street->isMonopoly())
    {
        throw std::runtime_error("Monopoly required.");
    }

    std::vector<Property *> group = board.getPropertiesByColorGroup(street->getColorGroup());
    int minLevel = street->getBuildingLevel();
    for (std::size_t i = 0; i < group.size(); ++i)
    {
        Street *groupStreet = dynamic_cast<Street *>(group[i]);
        if (groupStreet != 0)
        {
            minLevel = std::min(minLevel, groupStreet->getBuildingLevel());
        }
    }
    if (street->getBuildingLevel() > minLevel)
    {
        throw std::runtime_error("Buildings must be distributed evenly.");
    }

    int cost = street->getBuildingLevel() >= 4 ? street->getHotelCost() : street->getHouseCost();
    cost = GameManagerInternal::applyDiscount(player, cost);
    if (!bank.canPlayerAfford(player, cost))
    {
        if (display != 0)
        {
            display->printMessage("Uang kamu tidak cukup untuk membangun di " + GameManagerInternal::prettyName(street->getName()) + ".");
            display->printMessage("Biaya: " + GameManagerInternal::formatMoney(cost) + " | Uang kamu: " + GameManagerInternal::formatMoney(player.getMoney()));
            return;
        }
        throw std::runtime_error("Not enough money to build.");
    }

    const bool upgradeHotel = street->getBuildingLevel() >= 4;
    if (upgradeHotel && display != 0)
    {
        std::string answer = GameManagerInternal::toUpper(
            display->getInput("Upgrade ke hotel? Biaya: " + GameManagerInternal::formatMoney(cost) + " (y/n): "));
        if (answer != "Y" && answer != "YES")
        {
            return;
        }
    }

    bank.collectFromPlayer(player, cost);
    street->build();
    logger.log(currentTurn, player.getName(), "BUILD", street->getCode());
    if (display != 0)
    {
        if (upgradeHotel)
        {
            display->printMessage(GameManagerInternal::prettyName(street->getName()) + " di-upgrade ke Hotel!");
        }
        else
        {
            display->printMessage("Kamu membangun 1 rumah di " + GameManagerInternal::prettyName(street->getName()) + ". Biaya: " + GameManagerInternal::formatMoney(cost));
        }
        display->printMessage("Uang tersisa: " + GameManagerInternal::formatMoney(player.getMoney()));
    }
}
