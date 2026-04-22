#include "../../include/models/TaxTile.hpp"
#include "../../include/models/Player.hpp"
#include "../../include/core/GameContext.hpp"
#include "../../include/models/Property.hpp"
#include "../../include/models/Street.hpp"

TaxTile::TaxTile(int position, const std::string& name, const std::string& color,
                 TaxType taxType, int flatAmount, double percentageRate)
    : ActionTile(position, name, color, ActionType::TAX),
      taxType(taxType),
      flatAmount(flatAmount),
      percentageRate(percentageRate) {}

TaxType TaxTile::getTaxType() const { return taxType; }
int TaxTile::getFlatAmount() const { return flatAmount; }
double TaxTile::getPercentageRate() const { return percentageRate; }
void TaxTile::setTaxType(TaxType type) { taxType = type; }
void TaxTile::setFlatAmount(int amount) { if (amount >= 0) flatAmount = amount; }
void TaxTile::setPercentageRate(double rate) { if (rate >= 0.0) percentageRate = rate; }

int TaxTile::calculateTax(Player* player) const {
    if (player == nullptr) return 0;

    if (taxType == PBM) {
        return flatAmount;
    }

    // PPH — player pilih flat atau persentase
    // Keputusan pilihan ada di GameController/UI, bukan di sini
    // Method ini hanya hitung nilai masing-masing opsi
    return flatAmount; // default, GameController yang tentukan opsi mana
}

int TaxTile::calculateTaxPercentage(Player* player, GameContext* ctx) const {
    if (player == nullptr || ctx == nullptr) return 0;

    // Hitung total kekayaan: uang + harga beli semua properti + harga bangunan
    int totalWealth = player->getMoney();

    if (ctx->board != nullptr) {
        for (Tile* tile : ctx->board->getTiles()) {
            Property* prop = dynamic_cast<Property*>(tile);
            if (prop != nullptr && prop->getOwner() == player->getUsername()) {
                totalWealth += prop->getPurchasePrice();

                // Tambah harga bangunan kalau Street
                Street* street = dynamic_cast<Street*>(tile);
                if (street != nullptr) {
                    std::string bc = street->getBuildingCount();
                    if (bc == "H") {
                        totalWealth += street->getHotelPrice();
                    } else {
                        try {
                            int count = std::stoi(bc);
                            totalWealth += count * street->getHousePrice();
                        } catch (...) {}
                    }
                }
            }
        }
    }

    return static_cast<int>(totalWealth * percentageRate);
}

void TaxTile::collectTax(Player* player) {
    if (player == nullptr) return;
    int tax = calculateTax(player);
    if (tax > 0) {
        (*player) -= tax;
    }
}

void TaxTile::executeAction(Player* player, GameContext* ctx) {
    // PPH butuh pilihan dari player — GameController yang handle
    // PBM langsung potong
    if (player == nullptr) return;

    if (taxType == PBM) {
        collectTax(player);
    }
    // PPH: GameController yang panggil calculateTax() dan calculateTaxPercentage()
    // setelah dapat pilihan dari player via UI
}