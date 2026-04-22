#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Forward declarations — hindari circular include
class Card;
class SkillCard;
class Property;
class Tile;

enum class PlayerStatus {
    ACTIVE,
    BANKRUPT,
    JAILED
};

class Player {
private:
    // ── Identitas ────────────────────────────────
    string username;  // nama tampilan

    // ── Keuangan ─────────────────────────────────
    int balance;

    // ── Posisi ────────────────────────────────────
    int   position;   // indeks petak (0–39), dipakai GameMaster::movePlayer
    Tile* currPetak;  // pointer ke tile saat ini, dipakai Board & GameMaster

    // ── Status ────────────────────────────────────
    PlayerStatus status;
    int jailTurns;    // sudah berapa giliran di penjara (0–3)

    // ── Properti ──────────────────────────────────
    vector<Property*> properties;

    // ── Kartu ─────────────────────────────────────
    vector<SkillCard*> skillCards;  // kartu kemampuan di tangan (maks 3)
    bool cardUsedThisTurn;

    // ── Shield ────────────────────────────────────
    bool shieldActive;

public:
    // ── Konstruktor & destruktor ─────────────────
    Player(const string& username, int startingBalance);
    ~Player();

    // ── Identitas ────────────────────────────────
    string getUsername() const;

    // ── Keuangan ─────────────────────────────────
    int  getBalance() const;
    int  getMoney()   const;      // alias getBalance() — kompatibel kode lama
    int  getWealth()  const;      // balance + harga beli semua properti + bangunan

    Player& operator+=(int amount);
    Player& operator-=(int amount);
    bool    canAfford(int amount) const;
    bool    operator>(const Player& other) const;
    bool    operator<(const Player& other) const;

    // ── Posisi ────────────────────────────────────
    int   getPosition()  const;
    Tile* getCurrPetak() const;     // dipakai GameMaster::movePlayer
    void  setPosition(int tileIndex);
    void  setCurrPetak(Tile* tile); // dipakai GameMaster::movePlayer
    void  move(int steps);          // update position & currPetak — impl menunggu Board

    // ── Status ────────────────────────────────────
    PlayerStatus getStatus()          const;
    void         setStatus(PlayerStatus s);
    string       getStatusString()    const; // return "ACTIVE"/"JAILED"/"BANKRUPT"

    // ── Penjara ───────────────────────────────────
    int  getJailTurns()       const;
    void setJailTurns(int turns);
    void goToJail();              // set JAILED, jailTurns = 0, dipakai JailTile
    void releaseFromJail();       // set ACTIVE, jailTurns = 0
    bool isInJail()           const;
    void incrementJailTurns();

    // ── Properti ──────────────────────────────────
    void                     addProperty(Property* prop);
    void                     removeProperty(Property* prop);
    Property*                getPropertyAt(int pos)    const; // 0-based, dipakai GameMaster
    const vector<Property*>& getProperties()           const;
    int                      getPropertyCount()        const;
    int                      getPropertyNum()          const; // alias getPropertyCount()

    // ── Kartu Kemampuan ───────────────────────────
    bool                      addSkillCard(SkillCard* card); // return false jika tangan penuh (>3)
    void                      discardSkillCard(int index);
    SkillCard*                getSkillCardAt(int index) const; // dipakai Command
    const vector<SkillCard*>& getHand()                const;
    int                       getHandSize()            const;
    int                       getCardNum()             const; // alias getHandSize()
    void                      setCardUsedThisTurn(bool used);
    bool                      hasUsedCardThisTurn()    const;

    // ── Shield ────────────────────────────────────
    void activateShield();
    void deactivateShield();
    bool isShielded() const;

    // ── Turn lifecycle ────────────────────────────
    void onTurnStart(); // reset cardUsedThisTurn & shieldActive
};

#endif