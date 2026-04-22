#include "../include/models/FestivalTile.hpp"
#include "../include/models/Property.hpp"
#include "../include/models/Player.hpp"
#include "../include/core/GameContext.hpp"
#include "../include/models/GameBoard.hpp"

FestivalTile::FestivalTile(int position, const std::string& name, const std::string& color)
    : ActionTile(position, name, color, ActionType::FESTIVAL) {}

void FestivalTile::applyFestivalEffect(Player* player, Property* property) {
    if (player == nullptr || property == nullptr) return;

    int currentMult = property->getFestivalMultiplier();

    if (currentMult < 8) {
        property->setFestivalMultiplier(currentMult * 2);
    }
    // Kalau  max (8x) atau baru pertama (1x), reset durasi
    property->setFestivalDuration(3);
}

void FestivalTile::updateDuration() {
    // Dipanggil oleh GameController setiap akhir giliran player
    // Iterasi properti dilakukan di GameController, bukan di sini
}

void FestivalTile::executeAction(Player* player, GameContext* ctx) {
    // TODO: input dari UI untuk pilih properti
    // butuh akses list properti milik player dari board
    // GameController yang handle interaksi, tile cuma trigger
}