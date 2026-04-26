#include "core/game_manager/GameManager.hpp"

#include <sstream>
#include <stdexcept>

#include "../GameManagerInternal.hpp"
#include "models/base/Property.hpp"

void GameManager::commandUnmortgage(const std::string& propertyCode) {
    Player& player = getCurrentPlayer();
    std::string code = GameManagerInternal::toUpper(propertyCode);

    if (code.empty() && display != 0) {
        std::vector<Property*> mortgaged;
        std::ostringstream oss;
        oss << "=== Properti yang Sedang Digadaikan ===\n";
        for (std::size_t i = 0; i < player.getOwnedProperties().size(); ++i) {
            Property* property = player.getOwnedProperties()[i];
            if (property != 0 && property->isMortgaged()) {
                mortgaged.push_back(property);
                oss << mortgaged.size() << ". "
                    << GameManagerInternal::prettyName(property->getName())
                    << " (" << property->getCode() << ")"
                    << " [" << GameManagerInternal::propertyGroupLabel(*property) << "] [M] "
                    << "Harga Tebus: "
                    << GameManagerInternal::formatMoney(GameManagerInternal::applyDiscount(player, property->getMortgageValue() * 2))
                    << '\n';
            }
        }

        if (mortgaged.empty()) {
            display->printMessage("Tidak ada properti yang sedang digadaikan.");
            return;
        }

        display->printMessage(oss.str());
        display->printMessage("Uang kamu saat ini: " + GameManagerInternal::formatMoney(player.getMoney()));
        int choice = std::atoi(display->getInput("Pilih nomor properti (0 untuk batal): ").c_str());
        if (choice <= 0 || static_cast<std::size_t>(choice) > mortgaged.size()) {
            return;
        }
        code = mortgaged[static_cast<std::size_t>(choice - 1)]->getCode();
    }

    Property* property = findPropertyOrThrow(code);
    if (property->getOwner() != &player || !property->isMortgaged()) {
        throw std::runtime_error("Property is not mortgaged by current player.");
    }

    int cost = GameManagerInternal::applyDiscount(player, property->getMortgageValue() * 2);
    if (!bank.canPlayerAfford(player, cost)) {
        if (display != 0) {
            display->printMessage("Uang kamu tidak cukup untuk menebus "
                + GameManagerInternal::prettyName(property->getName()) + ".");
            display->printMessage("Harga tebus: " + GameManagerInternal::formatMoney(cost)
                + " | Uang kamu: " + GameManagerInternal::formatMoney(player.getMoney()));
            return;
        }
        throw std::runtime_error("Not enough money to unmortgage.");
    }

    bank.collectFromPlayer(player, cost);
    property->unmortgage();
    logger.log(currentTurn, player.getName(), "UNMORTGAGE", property->getCode());
    if (display != 0) {
        display->printMessage(GameManagerInternal::prettyName(property->getName()) + " berhasil ditebus!");
        display->printMessage("Kamu membayar " + GameManagerInternal::formatMoney(cost) + " ke Bank.");
        display->printMessage("Uang kamu sekarang: " + GameManagerInternal::formatMoney(player.getMoney()));
    }
}
