#include "core/game_manager/GameManager.hpp"

#include <sstream>
#include <stdexcept>
#include <vector>

#include "../GameManagerInternal.hpp"
#include "models/base/Property.hpp"
#include "models/tiles/Street.hpp"

namespace {

int buildingSaleValue(const Street& street) {
    int total = 0;
    for (int built = 0; built < street.getBuildingLevel(); ++built) {
        total += ((built < 4) ? street.getHouseCost() : street.getHotelCost()) / 2;
    }
    return total;
}

}

void GameManager::commandMortgage(const std::string& propertyCode) {
    Player& player = getCurrentPlayer();
    std::string code = GameManagerInternal::toUpper(propertyCode);

    if (code.empty() && display != 0) {
        std::vector<Property*> mortgageable;
        std::ostringstream oss;
        oss << "=== Properti yang Dapat Digadaikan ===\n";
        for (std::size_t i = 0; i < player.getOwnedProperties().size(); ++i) {
            Property* property = player.getOwnedProperties()[i];
            if (property != 0 && !property->isMortgaged()) {
                mortgageable.push_back(property);
                oss << mortgageable.size() << ". "
                    << GameManagerInternal::prettyName(property->getName())
                    << " (" << property->getCode() << ")"
                    << " [" << GameManagerInternal::propertyGroupLabel(*property) << "] "
                    << "Nilai Gadai: " << GameManagerInternal::formatMoney(property->getMortgageValue()) << '\n';
            }
        }

        if (mortgageable.empty()) {
            display->printMessage("Tidak ada properti yang dapat digadaikan saat ini.");
            return;
        }

        display->printMessage(oss.str());
        int choice = std::atoi(display->getInput("Pilih nomor properti (0 untuk batal): ").c_str());
        if (choice <= 0 || static_cast<std::size_t>(choice) > mortgageable.size()) {
            return;
        }
        code = mortgageable[static_cast<std::size_t>(choice - 1)]->getCode();
    }

    Property* property = findPropertyOrThrow(code);
    if (property->getOwner() != &player) {
        throw std::runtime_error("Property not owned by current player.");
    }
    if (property->isMortgaged()) {
        throw std::runtime_error("Properti tersebut sudah digadaikan.");
    }

    Street* street = dynamic_cast<Street*>(property);
    if (street != 0) {
        std::vector<Property*> sameGroup = board.getPropertiesByColorGroup(street->getColorGroup());
        bool groupHasBuildings = false;
        for (std::size_t i = 0; i < sameGroup.size(); ++i) {
            Street* sameStreet = dynamic_cast<Street*>(sameGroup[i]);
            if (sameStreet != 0 && sameStreet->hasBuildings()) {
                groupHasBuildings = true;
                break;
            }
        }

        if (groupHasBuildings) {
            if (display != 0) {
                std::ostringstream oss;
                oss << GameManagerInternal::prettyName(property->getName()) << " tidak dapat digadaikan!\n";
                oss << "Masih terdapat bangunan di color group [" << GameManagerInternal::propertyGroupLabel(*street) << "].\n";
                oss << "Bangunan harus dijual terlebih dahulu.\n\n";
                oss << "Daftar bangunan di color group [" << GameManagerInternal::propertyGroupLabel(*street) << "]:\n";
                for (std::size_t i = 0; i < sameGroup.size(); ++i) {
                    Street* sameStreet = dynamic_cast<Street*>(sameGroup[i]);
                    if (sameStreet != 0 && sameStreet->hasBuildings()) {
                        oss << i + 1 << ". "
                            << GameManagerInternal::prettyName(sameStreet->getName())
                            << " (" << sameStreet->getCode() << ")"
                            << " - " << GameManagerInternal::buildingLevelLabel(sameStreet->getBuildingLevel())
                            << " -> Nilai jual bangunan: " << GameManagerInternal::formatMoney(buildingSaleValue(*sameStreet)) << '\n';
                    }
                }
                display->printMessage(oss.str());
                std::string sell = GameManagerInternal::toUpper(
                    display->getInput("Jual semua bangunan color group [" + GameManagerInternal::propertyGroupLabel(*street) + "]? (y/n): "));
                if (sell != "Y" && sell != "YES") {
                    return;
                }
            }

            for (std::size_t i = 0; i < sameGroup.size(); ++i) {
                Street* sameStreet = dynamic_cast<Street*>(sameGroup[i]);
                if (sameStreet == 0) {
                    continue;
                }
                const int saleTotal = buildingSaleValue(*sameStreet);
                while (sameStreet->hasBuildings()) {
                    int refund = sameStreet->getBuildingLevel() >= 5 ? sameStreet->getHotelCost() : sameStreet->getHouseCost();
                    bank.payPlayer(player, refund / 2);
                    sameStreet->sellOneBuilding();
                }
                if (display != 0 && saleTotal > 0) {
                    display->printMessage("Bangunan " + GameManagerInternal::prettyName(sameStreet->getName())
                        + " terjual. Kamu menerima " + GameManagerInternal::formatMoney(saleTotal) + ".");
                }
            }

            if (display != 0) {
                display->printMessage("Uang kamu sekarang: " + GameManagerInternal::formatMoney(player.getMoney()));
                std::string proceed = GameManagerInternal::toUpper(
                    display->getInput("Lanjut menggadaikan " + GameManagerInternal::prettyName(property->getName()) + "? (y/n): "));
                if (proceed != "Y" && proceed != "YES") {
                    return;
                }
            }
        }
    }

    property->mortgage();
    bank.payPlayer(player, property->getMortgageValue());
    logger.log(currentTurn, player.getName(), "MORTGAGE", property->getCode());
    if (display != 0) {
        display->printMessage(GameManagerInternal::prettyName(property->getName()) + " berhasil digadaikan.");
        display->printMessage("Kamu menerima " + GameManagerInternal::formatMoney(property->getMortgageValue()) + " dari Bank.");
        display->printMessage("Uang kamu sekarang: " + GameManagerInternal::formatMoney(player.getMoney()));
        display->printMessage("Catatan: Sewa tidak dapat dipungut dari properti yang digadaikan.");
    }
}
