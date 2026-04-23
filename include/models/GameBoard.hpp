#pragma once
#include <vector>
#include <string>

class Tile;
class Player;
class SkillCard;
template <typename T> class CardDeck;

class GameBoard {
private:
    std::vector<Tile*> tiles;
    std::vector<Player*> players;
    int currentPlayerIndex;
    int currentTurnNumber;
    int maxTurn;
    CardDeck<SkillCard>* skillDeck;

public:
    GameBoard();


    void addTile(Tile* tile);
    Tile* getTileAt(int position) const;

    void addPlayer(Player* player);
    Player* getPlayerByUsername(const std::string& username) const;
    Player* getCurrentPlayer() const;

    void nextTurn();

    const std::vector<Player*>& getPlayers() const;
    const std::vector<Tile*>& getTiles() const;

    int getCurrentTurnNumber() const;
    void setCurrentTurnNumber(int t);
    int getMaxTurn() const;
    void setMaxTurn(int m);

    CardDeck<SkillCard>* getSkillDeck() const;
    void setSkillDeck(CardDeck<SkillCard>* d);

    void setCurrentPlayerByUsername(const std::string& username);
    void setTurnOrder(const std::vector<std::string>& order);
};
