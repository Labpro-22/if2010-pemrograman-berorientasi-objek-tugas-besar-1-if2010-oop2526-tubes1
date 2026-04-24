#pragma once
#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include "core/foundation/Bank.hpp"
#include "core/foundation/Board.hpp"
#include "core/foundation/CardDeck.hpp"
#include "core/foundation/Dice.hpp"
#include "models/cards/ChanceCard.hpp"
#include "models/cards/CommunityChestCard.hpp"
#include "models/base/GameTypes.hpp"
#include "models/player/Player.hpp"
#include "models/cards/SkillCard.hpp"
#include "utils/data/ConfigManager.hpp"
#include "utils/io/DisplayHandler.hpp"
#include "utils/data/TransactionLogger.hpp"

class Property;
class Street;
class Action;
class Special;

class GameManager {
    friend class AuctionSession;
    friend class CardEffectResolver;
    friend class DebtResolver;
    friend class Action;
    friend class Property;
    friend class Special;

private:
    std::vector<Player*> players;
    Board board;
    Bank bank;
    Dice dice;
    TransactionLogger logger;
    DisplayHandler* display;
    ConfigManager configManager;
    GameConfig gameConfig;

    CardDeck<ChanceCard> chanceDeck;
    CardDeck<CommunityChestCard> communityDeck;
    CardDeck<SkillCard> skillDeck;
    std::vector<SkillCard*> discardedSkillCards;

    int currentPlayerIndex;
    int currentTurn;
    int consecutiveDoubles;
    bool useManualDice;
    std::pair<int, int> manualDiceValue;
    bool hasRolledThisTurn;
    bool hasUsedSkillThisTurn;
    TurnPhase turnPhase;
    bool turnShouldEnd;
    bool currentRollWasDouble;

    static GameManager* instance;

    GameManager();

    void loadConfig(const std::string& configDirectory);
    void buildDefaultBoard();
    void buildDefaultChanceDeck();
    void buildDefaultCommunityDeck();
    void buildDefaultSkillDeck();
    void createPlayers(const std::vector<std::string>& playerNames);

    void clearPlayers();
    void clearDiscardedSkillCards();
    Property* findPropertyOrThrow(const std::string& propertyCode) const;
    Player* findPlayerByName(const std::string& playerName) const;
    void refreshFestivalStates(Player& player);
    void handleAutomaticSkillDrop(Player& player);
    GameSaveData buildSaveData() const;
    void applySaveData(const GameSaveData& saveData);

    void gameLoop();
    void playTurn();

    void commandHelp() const;
    void commandPrintBoard() const;
    std::pair<int, int> commandRollDice();
    void commandSetDice(int die1, int die2);
    void commandNext();
    void commandPayJailFine();
    void commandPrintDeed(const std::string& propertyCode) const;
    void commandPrintProperties() const;
    void commandBuy();
    void commandMortgage(const std::string& propertyCode);
    void commandUnmortgage(const std::string& propertyCode);
    void commandBuild(const std::string& propertyCode);
    void commandSave(const std::string& saveFilePath) const;
    void commandPrintLog(int lastEntries = -1) const;
    void commandUseSkillCard(std::size_t skillCardIndex);
    void commandDropSkillCard(std::size_t skillCardIndex);
    void commandAnnounceWinner() const;

    std::pair<int, int> rollMovementDice();
    void setManualDice(int die1, int die2);
    void clearManualDice();
    void movePlayer(Player& player, int steps);
    void movePlayerTo(Player& player, int targetPosition, bool collectGoSalary = true);
    void resolveLanding(Player& player, int diceTotal);

    void handlePropertyLanding(Player& player, Property& property, int diceTotal);
    void handleStreetPurchase(Player& player, Street& street);
    void handleRentPayment(Player& visitor, Property& property, int diceTotal);
    void handleAuction(Property& property);
    void handleFestival(Player& player);
    void handleTax(Player& player, ActionType taxType);
    void handleJail(Player& player);
    void handleBankruptcy(Player& player, Player* creditor);
    void drawChanceCard(Player& player);
    void drawCommunityChestCard(Player& player);
    SkillCard* drawSkillCard();
    void discardSkillCard(SkillCard* skillCard);
    void rebuildSkillDeckFromDiscard();

public:
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    ~GameManager();

    static GameManager* getInstance();

    void setDisplayHandler(DisplayHandler* displayHandler);
    void startNewGame(const std::vector<std::string>& playerNames, const std::string& configDirectory);
    void loadSavedGame(const std::string& saveFilePath, const std::string& configDirectory);
    void saveGame(const std::string& saveFilePath) const;

    void start();
    void endTurn();
    void handleCommand(const std::string& commandLine);

    Player& getCurrentPlayer();
    const std::vector<Player*>& getPlayers() const;
    Board& getBoard();
    const Board& getBoard() const;
    Bank& getBank();
    const Bank& getBank() const;
    TransactionLogger& getLogger();
    const TransactionLogger& getLogger() const;
    const GameConfig& getGameConfig() const;
    const CardDeck<SkillCard>& getSkillDeck() const;
    const std::vector<SkillCard*>& getDiscardedSkillCards() const;
    bool getHasRolledThisTurn() const;
    bool getHasUsedSkillThisTurn() const;
    TurnPhase getTurnPhase() const;
    bool getCurrentRollWasDouble() const;
    void setTurnStateForTesting(TurnPhase phase, bool shouldEnd, bool rollWasDouble);
    void awardSkillCard(Player& player);
    void tickPlayerSkillDurations(Player& player);

    // Common read-only helpers
    int getGoSalary() const;
    int getJailFine() const;
    int getMaxTurn() const;
    bool isGameOver() const;
};
