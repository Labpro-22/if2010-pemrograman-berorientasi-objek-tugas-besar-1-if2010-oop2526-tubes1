#pragma once
#include "Saveable.hpp"

class Player;
class CardTile;
class TaxTile;
class FestivalTile;
class GoTile;
class GoToJailTile;
class FreeParkingTile;
class JailTile;
class StreetTile;
class RailroadTile;
class UtilityTile;

class TileVisitor : public Saveable{ //sebenernya butuh di GameManager but ternyata parentnya GameManager tuh ini
public:
    virtual ~TileVisitor() = default;

    // Setiap fungsi ini nantinya akan diimplementasikan oleh GameManager
    
    virtual void visitCardTile(CardTile* tile, Player& player) = 0;
    virtual void visitTaxTile(TaxTile* tile, Player& player) = 0;
    virtual void visitFestivalTile(FestivalTile* tile, Player& player) = 0;
    virtual void visitGoTile(GoTile* tile, Player& player) = 0;
    virtual void visitGoToJailTile(GoToJailTile* tile, Player& player) = 0;
    virtual void visitFreeParkingTile(FreeParkingTile* tile, Player& player) = 0;
    virtual void visitJailTile(JailTile* tile, Player& player) = 0;
    // Untuk properti, dipisahkan menjadi 3 fungsi spesifik agar GameManager tahu pasti jenis properti apa yang diinjak tanpa perlu menggunakan dynamic_cast
    virtual void visitStreetTile(StreetTile* tile, Player& player) = 0;
    virtual void visitRailroadTile(RailroadTile* tile, Player& player) = 0;
    virtual void visitUtilityTile(UtilityTile* tile, Player& player) = 0;
};