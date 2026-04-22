# Nimonspoli — Comprehensive Class Design

## Architecture Overview

The system follows a strict **3-Layer Architecture**:

1. **UI Layer** — All input/output. Zero game logic.
2. **Game Logic Layer** — Core rules, state, calculations. Zero I/O.
3. **Data Access Layer** — File reading/writing. Zero game logic.

Layers communicate through **interfaces** (abstract classes), enabling CLI/GUI swap and Human/COM swap without touching game logic.

---

## Architectural Patterns (7 Pillars)

These patterns elevate the design from "organized code" to "extensible architecture". Every bonus and future feature slots in without modifying existing classes (Open/Closed Principle).

### Pillar 1: Command Pattern (21 Commands)

Each user command is its own class, not a switch-case branch.

```
Command (abstract)
  # name: string
  + virtual execute(engine: GameEngine&, player: Player&) = 0
  + virtual canExecute(state: GameStateView&): bool = 0
  + virtual getName(): string
  + virtual getHelp(): string

├── RollDiceCommand            ├── BuildCommand
├── SetDiceCommand              ├── MortgageCommand
├── PrintBoardCommand           ├── RedeemCommand
├── PrintDeedCommand            ├── UseSkillCardCommand
├── PrintPropertiesCommand      ├── SaveCommand
├── PrintLogCommand             └── LoadCommand
```

```
class CommandRegistry {
  - commands: map<string, Command*>
  + registerCommand(cmd: Command*)
  + parseAndExecute(input: string, engine: GameEngine&, player: Player&)
  + getAvailableCommands(state: GameStateView&): vector<string>
}
```

**Benefit:** Adding a creativity command = 1 new file, zero edits.

### Pillar 2: Observer Pattern (Event-Driven UI)

`GameEngine` emits events; multiple views subscribe.

```
class GameEvent (abstract) { + getType(): EventType }
├── PlayerMovedEvent { player, fromTile, toTile }
├── MoneyChangedEvent { player, oldAmount, newAmount, reason }
├── PropertyAcquiredEvent { player, property, price }
├── RentPaidEvent { payer, owner, amount, property }
├── BuildingBuiltEvent, MortgagedEvent, RedeemedEvent
├── FestivalActivatedEvent, AuctionStartedEvent, BidPlacedEvent
├── CardDrawnEvent, SkillCardUsedEvent, EffectAppliedEvent
├── BankruptcyEvent, TurnStartedEvent, TurnEndedEvent
└── GameOverEvent

class IEventListener (abstract) { + onEvent(event: GameEvent&) = 0 }

class EventBus {
  - listeners: map<EventType, vector<IEventListener*>>
  + subscribe(type: EventType, listener: IEventListener*)
  + publish(event: GameEvent&)
}
```

`CLIView`, `GUIBoardPanel`, `GUIPlayerPanel`, `GUILogPanel`, even `TransactionLogger` all subscribe. Game Logic publishes once — every UI piece updates independently.

### Pillar 3: Effect System (Polymorphic Modifiers)

Replace ad-hoc bool fields on Player with a polymorphic effect list.

```
class Effect (abstract) {
  # turnsRemaining: int
  + virtual onTurnStart(player: Player&)
  + virtual onTurnEnd(player: Player&)
  + virtual modifyIncomingPayment(amount: Money): Money
  + virtual modifyOutgoingPayment(amount: Money): Money
  + virtual modifyRentReceived(amount: Money, prop: PropertyTile*): Money
  + virtual blocksPayment(): bool
  + virtual isExpired(): bool
  + virtual getDescription(): string
}
├── ShieldEffect          (blocksPayment = true, 1 turn)
├── DiscountEffect        (modifyOutgoingPayment * (100-pct)/100, 1 turn)
├── FestivalEffect        (modifyRentReceived * multiplier, 3 turns, per-property)
└── (creativity bonus: any new effect just subclasses Effect)
```

```
class Player {
  - activeEffects: vector<Effect*>
  + addEffect(effect: Effect*)
  + tickEffects()                    // calls onTurnEnd, removes expired
  + applyOutgoingModifiers(amount: Money): Money
  + applyIncomingModifiers(amount: Money): Money
  + isPaymentBlocked(): bool
}
```

**Benefit:** New skill cards (creativity bonus) = new Effect subclass. Player class never changes. Open/Closed achieved.

### Pillar 4: Builder Pattern (Game Construction)

Separates "load config" from "build game". One clear construction sequence.

```
class GameBuilder {
  - config: GameConfig
  - board: Board*
  - players: vector<Player*>
  - cardSystem: CardSystem*
  - eventBus: EventBus*

  + loadConfig(configDir: string): GameBuilder&
  + buildBoard(): GameBuilder&                 // uses BoardFactory
  + buildCardDecks(): GameBuilder&             // uses CardSystemBuilder
  + addHumanPlayer(name: string): GameBuilder&
  + addComputerPlayer(name: string, ai: AILevel): GameBuilder&
  + setView(view: IGameView*): GameBuilder&
  + randomizeTurnOrder(): GameBuilder&
  + loadFromSave(filepath: string): GameBuilder&
  + build(): GameEngine*                       // assembles everything
}
```

Usage in `main.cpp`:
```cpp
GameEngine* game = GameBuilder()
    .loadConfig("config/")
    .buildBoard()
    .buildCardDecks()
    .addHumanPlayer("Alice")
    .addComputerPlayer("BotBob", AILevel::SMART)
    .setView(new CLIView())
    .randomizeTurnOrder()
    .build();
game->runGameLoop();
```

### Pillar 5: Repository Pattern (Persistence Abstraction)

Decouples "what to save" from "where/how to save".

```
class IGameRepository (abstract) {
  + virtual save(state: GameState&, identifier: string): bool = 0
  + virtual load(identifier: string): GameState = 0
  + virtual exists(identifier: string): bool = 0
}
├── TextFileRepository     (current spec — .txt format)
├── JsonRepository         (creativity bonus future-proof)
└── LeaderboardRepository  (creativity bonus: account-based)
```

`GameEngine` holds `IGameRepository*`, never knows file formats.

### Pillar 6: Interface Segregation (Focused Controllers)

Split the fat `PlayerController` into focused decision interfaces.

```
class IBuyDecisionMaker  { + decideBuyProperty(...) = 0 }
class IAuctionStrategy   { + decideAuction(...), decideBid(...) = 0 }
class IBuildStrategy     { + decideBuild(...), decideMortgage(...) = 0 }
class ITaxStrategy       { + decideTaxOption(...) = 0 }
class ICardStrategy      { + decideUseCard(...), decideDropCard(...),
                            decideTeleport(...), decideLasso(...),
                            decideDemolition(...) = 0 }
class ILiquidationStrategy { + decideLiquidation(...) = 0 }
class IJailStrategy      { + decideJailAction(...) = 0 }

class PlayerController : public IBuyDecisionMaker, IAuctionStrategy,
                         IBuildStrategy, ITaxStrategy, ICardStrategy,
                         ILiquidationStrategy, IJailStrategy {
  // Aggregates all strategy interfaces via multiple inheritance of pure interfaces
}

├── HumanController      (delegates each to IGameInput)
├── BasicAIController    (simple heuristics)
└── SmartAIController    (advanced strategy — bonus depth)
```

**Benefit:** Different AI difficulties can mix strategies. Testing each strategy in isolation.

### Pillar 7: Immutable State View (Encapsulation Safety)

COM controllers and views receive read-only snapshots.

```
class GameStateView {                          // Immutable
  - const data: GameStateData
  + getCurrentTurn(): int
  + getPlayers(): vector<PlayerView>           // PlayerView also read-only
  + getBoard(): BoardView
  + getMyPlayer(name: string): PlayerView
  // No setters, no mutators
}

class GameState {                              // Mutable, owned by GameEngine only
  + toView(): GameStateView                    // produces snapshot
  // ... mutators
}
```

COM strategies receive `GameStateView` — cannot accidentally corrupt state. Only `GameEngine` mutates.

---

## Layer 1: UI Layer

### `IGameView` (Abstract)
Display interface. Game Logic calls this to show information — never `cout` directly.

```
class IGameView {
  virtual void showBoard(const BoardState&) = 0
  virtual void showDiceResult(int d1, int d2, string playerName) = 0
  virtual void showPlayerLanding(string playerName, string tileName) = 0
  virtual void showPropertyCard(const PropertyInfo&) = 0
  virtual void showPlayerProperties(const PlayerPropertyList&) = 0
  virtual void showBuyPrompt(const PropertyInfo&, int playerMoney) = 0
  virtual void showRentPayment(const RentInfo&) = 0
  virtual void showTaxPrompt(const TaxInfo&) = 0
  virtual void showAuctionState(const AuctionState&) = 0
  virtual void showFestivalPrompt(const vector<string>& ownedProperties) = 0
  virtual void showBankruptcy(const BankruptcyInfo&) = 0
  virtual void showLiquidationPanel(const LiquidationState&) = 0
  virtual void showCardDrawn(const CardInfo&) = 0
  virtual void showSkillCardHand(const vector<SkillCardInfo>&) = 0
  virtual void showTransactionLog(const vector<LogEntry>&) = 0
  virtual void showWinner(const WinInfo&) = 0
  virtual void showJailStatus(const JailInfo&) = 0
  virtual void showMessage(string message) = 0
  virtual void showBuildMenu(const BuildMenuState&) = 0
  virtual void showMortgageMenu(const MortgageMenuState&) = 0
  virtual void showRedeemMenu(const RedeemMenuState&) = 0
  virtual void showDropCardPrompt(const vector<SkillCardInfo>&) = 0
  virtual void showSaveLoadStatus(string message) = 0
  virtual void showTurnInfo(string playerName, int turnNum, int maxTurn) = 0
}
```

**Concrete implementations:**
- `CLIView` — Terminal-based output with colored print (ANSI codes)
- `GUIView` — Raylib/SFML-based graphical rendering

### `IGameInput` (Abstract)
Input interface. Game Logic requests decisions through this.

```
class IGameInput {
  virtual int getMenuChoice(const vector<string>& options) = 0
  virtual string getCommand() = 0
  virtual bool getYesNo(string prompt) = 0
  virtual int getNumberInRange(string prompt, int min, int max) = 0
  virtual string getString(string prompt) = 0
  virtual pair<int,int> getManualDice() = 0
  virtual AuctionAction getAuctionAction(string playerName, int currentBid, int playerMoney) = 0
  virtual TaxChoice getTaxChoice() = 0
  virtual int getLiquidationChoice(int numOptions) = 0
  virtual int getSkillCardChoice(int numCards) = 0
  virtual string getPropertyCodeInput(string prompt) = 0
}
```

**Concrete implementations:**
- `CLIInput` — `cin`-based terminal input
- `GUIInput` — GUI event-driven input (buttons, clicks)

### `PlayerController` (Abstract)
Strategy pattern for decision-making. Decouples "who decides" from "what happens".

```
class PlayerController {
  virtual string chooseCommand(const GameState& visible) = 0
  virtual bool decideBuyProperty(const PropertyInfo&, int money) = 0
  virtual AuctionAction decideAuction(int currentBid, int money) = 0
  virtual TaxChoice decideTax(int flatAmount, int percentAmount) = 0
  virtual int decideSkillCard(const vector<SkillCardInfo>&) = 0
  virtual string decideFestivalProperty(const vector<string>&) = 0
  virtual int decideBuild(const BuildMenuState&) = 0
  virtual int decideLiquidation(const LiquidationState&) = 0
  virtual int decideDropCard(const vector<SkillCardInfo>&) = 0
  virtual bool decideJailPay() = 0
  virtual string decideTeleportTarget() = 0
  virtual string decideLassoTarget(const vector<string>&) = 0
  virtual string decideDemolitionTarget(const vector<PropertyInfo>&) = 0
}
```

**Concrete implementations:**
- `HumanController` — Delegates to `IGameInput` to ask the human
- `ComputerController` — AI logic: evaluates board state, makes strategic decisions

---

## Layer 2: Game Logic Layer

### Core Engine

#### `GameEngine`
Central orchestrator. Owns all managers and runs the game loop.

```
class GameEngine {
  - board: Board*
  - players: vector<Player*>
  - turnManager: TurnManager
  - auctionManager: AuctionManager
  - bankruptcyManager: BankruptcyManager
  - festivalManager: FestivalManager
  - transactionLogger: TransactionLogger
  - diceManager: Dice
  - cardSystem: CardSystem
  - bank: Bank
  - gameConfig: GameConfig
  - view: IGameView*
  - gameState: GameState

  + initialize(config: GameConfig, players: vector<PlayerSetup>)
  + runGameLoop()
  + processCommand(command: string)
  + isGameOver(): bool
  + getWinner(): vector<Player*>

  - executeTurn(player: Player*)
  - handleLanding(player: Player*, tile: Tile*)
  - handleDoubles(player: Player*, int consecutiveDoubles)
  - checkPassGo(player: Player*, int oldPos, int newPos)
  - processSkillCardPhase(player: Player*)
  - processDrawCardPhase(player: Player*)
}
```

#### `GameState`
Snapshot of the entire game state. Used for save/load and for COM visibility.

```
class GameState {
  + currentTurn: int
  + maxTurn: int
  + players: vector<PlayerState>
  + turnOrder: vector<string>
  + activePlayerIndex: int
  + propertyStates: vector<PropertyState>
  + deckState: DeckState
  + logEntries: vector<LogEntry>
  + hasRolledDice: bool
  + hasUsedSkillCard: bool
  + consecutiveDoubles: int
}
```

#### `GameConfig`
All configuration values loaded from files.

```
class GameConfig {
  + properties: map<string, PropertyConfig>
  + railroadRents: map<int, int>
  + utilityMultipliers: map<int, int>
  + pphFlat: int
  + pphPercentage: int
  + pbmFlat: int
  + goSalary: int
  + jailFine: int
  + maxTurn: int
  + startingBalance: int
  + boardLayout: vector<TileConfig>  // For dynamic board bonus
}
```

### Board & Tiles

#### `Board`
Container for tiles. Supports dynamic size (20-60 tiles).

```
class Board {
  - tiles: vector<Tile*>
  - tilesByCode: map<string, Tile*>
  - size: int

  + getTile(index: int): Tile*
  + getTileByCode(code: string): Tile*
  + getSize(): int
  + getNextTileOfType(fromIndex: int, type: TileType): int
  + findNearestStation(fromIndex: int): int
  + getGoIndex(): int
  + getJailIndex(): int
}
```

#### `Tile` (Abstract)
Base class for all tile types. Pure virtual `onLand()`.

```
class Tile {
  # index: int
  # code: string
  # name: string
  # type: TileType

  + getIndex(): int
  + getCode(): string
  + getName(): string
  + getType(): TileType
  + virtual onLand(player: Player&, context: GameContext&): LandingResult = 0
}
```

`TileType` enum: `STREET, RAILROAD, UTILITY, CHANCE, COMMUNITY_CHEST, FESTIVAL, TAX_PPH, TAX_PBM, GO, JAIL, FREE_PARKING, GO_TO_JAIL`

`GameContext` is a struct/reference bundle passed to tiles so they can query game state without coupling:
```
struct GameContext {
  Bank& bank
  FestivalManager& festivalManager
  TransactionLogger& logger
  GameConfig& config
  Dice& dice
  vector<Player*>& players
  Board& board
  int currentTurn
}
```

`LandingResult` enum/struct indicating what happened:
```
struct LandingResult {
  LandingAction action  // BUY_PROMPT, PAY_RENT, PAY_TAX, DRAW_CARD,
                        // FESTIVAL_PROMPT, GO_TO_JAIL, NOTHING,
                        // AUTO_ACQUIRE, BANKRUPT_CHECK
  int amount
  Player* creditor
  PropertyTile* property
  // ... other contextual data
}
```

#### Tile Hierarchy (Inheritance + Polymorphism)

```
Tile (abstract)
├── PropertyTile (abstract)
│   ├── StreetTile
│   ├── RailroadTile
│   └── UtilityTile
├── ActionTile (abstract)
│   ├── ChanceTile
│   ├── CommunityChestTile
│   ├── FestivalTile
│   └── TaxTile
│       ├── PPHTile
│       └── PBMTile
└── SpecialTile (abstract)
    ├── GoTile
    ├── JailTile
    ├── FreeParkingTile
    └── GoToJailTile
```

#### `PropertyTile` (Abstract)
Base for ownable tiles.

```
class PropertyTile : public Tile {
  # owner: Player*
  # status: PropertyStatus        // BANK, OWNED, MORTGAGED
  # purchasePrice: int
  # mortgageValue: int
  # festivalMultiplier: int       // 1, 2, 4, or 8
  # festivalDuration: int         // 0-3

  + getOwner(): Player*
  + setOwner(player: Player*)
  + getStatus(): PropertyStatus
  + setStatus(status: PropertyStatus)
  + getPurchasePrice(): int
  + getMortgageValue(): int
  + virtual calculateRent(context: GameContext&): int = 0
  + getFestivalMultiplier(): int
  + setFestivalEffect(multiplier: int, duration: int)
  + tickFestival()                // Decrement duration, reset if 0
  + isMonopolized(): bool         // Overridden by StreetTile
  + virtual getSellValue(): int   // For liquidation
}
```

#### `StreetTile`
```
class StreetTile : public PropertyTile {
  - colorGroup: ColorGroup
  - buildingLevel: int            // 0-4 houses, 5 = hotel
  - rentTable: vector<int>        // Index 0-5 (land, 1-4 houses, hotel)
  - houseCost: int
  - hotelCost: int

  + override calculateRent(context: GameContext&): int
  + override onLand(player: Player&, context: GameContext&): LandingResult
  + getColorGroup(): ColorGroup
  + getBuildingLevel(): int
  + setBuildingLevel(level: int)
  + canBuild(board: Board&): bool
  + getHouseCost(): int
  + getHotelCost(): int
  + override getSellValue(): int  // purchasePrice + half building costs
  + getRentTable(): vector<int>
}
```

#### `RailroadTile`
```
class RailroadTile : public PropertyTile {
  + override calculateRent(context: GameContext&): int  // Based on owner's railroad count
  + override onLand(player: Player&, context: GameContext&): LandingResult
}
```

#### `UtilityTile`
```
class UtilityTile : public PropertyTile {
  + override calculateRent(context: GameContext&): int  // diceTotal * multiplier
  + override onLand(player: Player&, context: GameContext&): LandingResult
}
```

#### `ColorGroup`
Manages a set of streets sharing the same color.

```
class ColorGroup {
  - color: Color                  // enum: BROWN, LIGHT_BLUE, PINK, ORANGE, RED, YELLOW, GREEN, DARK_BLUE
  - streets: vector<StreetTile*>

  + getColor(): Color
  + getStreets(): vector<StreetTile*>
  + isMonopolizedBy(player: Player*): bool
  + allHaveMinBuildings(level: int): bool
  + getMaxBuildingDifference(): int
  + getTotalBuildingValue(): int
  + hasAnyBuildings(): bool
}
```

### Player

#### `Money`
Value class wrapping monetary operations. Enables operator overloading.

```
class Money {
  - amount: int

  + Money(amount: int)
  + getAmount(): int
  + operator+(other: Money): Money
  + operator-(other: Money): Money
  + operator+=(other: Money): Money&
  + operator-=(other: Money): Money&
  + operator<(other: Money): bool
  + operator>(other: Money): bool
  + operator<=(other: Money): bool
  + operator>=(other: Money): bool
  + operator==(other: Money): bool
  + static zero(): Money
}
```

#### `Player`
```
class Player {
  - username: string
  - money: Money
  - position: int
  - status: PlayerStatus          // ACTIVE, BANKRUPT, JAILED
  - properties: vector<PropertyTile*>
  - skillCards: vector<SkillCard*>
  - controller: PlayerController*
  - jailTurnsRemaining: int
  - consecutiveDoubles: int
  - hasUsedSkillCardThisTurn: bool
  - hasRolledDiceThisTurn: bool
  - shieldActive: bool
  - discountPercent: int
  - discountTurnsLeft: int

  + getUsername(): string
  + getMoney(): Money
  + setMoney(money: Money)
  + addMoney(amount: Money)       // Uses operator+=
  + deductMoney(amount: Money)    // Uses operator-=
  + canAfford(amount: Money): bool
  + getPosition(): int
  + setPosition(pos: int)
  + getStatus(): PlayerStatus
  + setStatus(status: PlayerStatus)
  + getProperties(): vector<PropertyTile*>
  + addProperty(tile: PropertyTile*)
  + removeProperty(tile: PropertyTile*)
  + getSkillCards(): vector<SkillCard*>
  + addSkillCard(card: SkillCard*)
  + removeSkillCard(int index)
  + getController(): PlayerController*
  + isActive(): bool
  + isBankrupt(): bool
  + isJailed(): bool
  + getTotalWealth(): Money       // Cash + property prices + building costs
  + countRailroads(): int
  + countUtilities(): int
  + getPropertiesByColorGroup(color: Color): vector<StreetTile*>
  + hasMonopoly(color: Color): bool

  + operator<(other: Player&): bool   // Compare total wealth for rankings
  + operator>(other: Player&): bool
}
```

### Dice

#### `Dice`
```
class Dice {
  - die1: int
  - die2: int
  - isManual: bool

  + roll(): pair<int, int>        // Random roll
  + setManual(d1: int, d2: int)   // Manual set (ATUR_DADU)
  + getDie1(): int
  + getDie2(): int
  + getTotal(): int
  + isDouble(): bool
}
```

### Turn Management

#### `TurnManager`
```
class TurnManager {
  - turnOrder: vector<Player*>
  - currentIndex: int
  - currentTurn: int              // Increments after full cycle
  - maxTurn: int

  + getCurrentPlayer(): Player*
  + advanceToNextPlayer()
  + removePlayer(player: Player*)
  + getCurrentTurn(): int
  + isMaxTurnReached(): bool
  + getActivePlayers(): vector<Player*>
  + getActivePlayerCount(): int
  + getNextPlayerAfter(player: Player*): Player*
  + getTurnOrder(): vector<Player*>
  + setTurnOrder(order: vector<Player*>)
  + incrementTurn()
  + getPlayerIndex(player: Player*): int
}
```

### Bank

#### `Bank`
Unlimited money source/sink. Not a player.

```
class Bank {
  + payPlayer(player: Player&, amount: Money, reason: string)
  + collectFromPlayer(player: Player&, amount: Money, reason: string)
  + transferBetweenPlayers(from: Player&, to: Player&, amount: Money, reason: string)
  // Bank has unlimited money; these just modify player balances
}
```

### Card System

#### `CardDeck<T>` (Generic Class)
Templated deck that works for all card types.

```
template <typename T>
class CardDeck {
  - drawPile: vector<T*>
  - discardPile: vector<T*>

  + shuffle()
  + draw(): T*
  + discard(card: T*)
  + isEmpty(): bool
  + reshuffleDiscard()           // Move discard to draw, shuffle
  + getRemainingCount(): int
  + getDrawPile(): vector<T*>    // For save/load
  + setDrawPile(pile: vector<T*>)
}
```

**Instantiations:**
- `CardDeck<ChanceCard>`
- `CardDeck<CommunityChestCard>`
- `CardDeck<SkillCard>`

#### `Card` (Abstract Base)
```
class Card {
  # name: string
  # description: string

  + getName(): string
  + getDescription(): string
  + virtual getType(): string = 0
}
```

#### `ActionCard` (Abstract — for Chance & Community Chest)
```
class ActionCard : public Card {
  + virtual execute(player: Player&, context: GameContext&): CardResult = 0
}
```

#### Chance Cards
```
class ChanceCard : public ActionCard { ... }

class ChanceGoToStation : public ChanceCard {
  + override execute(...): CardResult   // Move to nearest station
}
class ChanceMoveBack3 : public ChanceCard {
  + override execute(...): CardResult   // Move back 3 tiles
}
class ChanceGoToJail : public ChanceCard {
  + override execute(...): CardResult   // Go to jail
}
```

#### Community Chest Cards
```
class CommunityChestCard : public ActionCard { ... }

class CommunityBirthday : public CommunityChestCard {
  + override execute(...): CardResult   // Get M100 from each player
}
class CommunityDoctorFee : public CommunityChestCard {
  + override execute(...): CardResult   // Pay M700
}
class CommunityElection : public CommunityChestCard {
  + override execute(...): CardResult   // Pay M200 to each player
}
```

#### Skill Cards
```
class SkillCard : public Card {
  + virtual activate(player: Player&, context: GameContext&): CardResult = 0
  + virtual getCardType(): SkillCardType = 0
  + virtual getSaveValue(): string = 0
  + virtual getSaveDuration(): string = 0
}
```

```
SkillCard (abstract)
├── MoveCard        { steps: int }
├── DiscountCard    { percentage: int, turnsLeft: int }
├── ShieldCard
├── TeleportCard
├── LassoCard
└── DemolitionCard
```

Each skill card implements `activate()`:
- **MoveCard**: Move player forward `steps` tiles
- **DiscountCard**: Set discount effect on player for 1 turn
- **ShieldCard**: Set shield active on player for 1 turn
- **TeleportCard**: Player chooses any tile to move to
- **LassoCard**: Player chooses opponent ahead to pull to current position
- **DemolitionCard**: Player chooses opponent's property to destroy

#### `CardSystem`
Manages all three decks.

```
class CardSystem {
  - chanceDeck: CardDeck<ChanceCard>
  - communityChestDeck: CardDeck<CommunityChestCard>
  - skillDeck: CardDeck<SkillCard>

  + drawChance(): ChanceCard*
  + drawCommunityChest(): CommunityChestCard*
  + drawSkill(): SkillCard*
  + discardSkill(card: SkillCard*)
  + initializeDecks()
  + getSkillDeck(): CardDeck<SkillCard>&    // For save/load
}
```

### Managers

#### `AuctionManager`
```
class AuctionManager {
  - currentBid: int
  - highestBidder: Player*
  - consecutivePasses: int
  - participants: vector<Player*>

  + startAuction(property: PropertyTile*, players: vector<Player*>,
                 triggerPlayer: Player*, context: GameContext&): AuctionResult
  - runAuctionLoop(view: IGameView*): AuctionResult
  - getNextBidder(): Player*
  - isAuctionOver(activeCount: int): bool
}
```

```
struct AuctionResult {
  Player* winner       // nullptr if no winner (shouldn't happen per spec)
  int finalPrice
  PropertyTile* property
}
```

#### `BankruptcyManager`
```
class BankruptcyManager {
  + checkBankruptcy(player: Player&, obligation: Money,
                    creditor: Player*, context: GameContext&): BankruptcyResult
  + calculateMaxLiquidation(player: Player&): Money
  + executeLiquidation(player: Player&, obligation: Money,
                       creditor: Player*, context: GameContext&)
  + executeBankruptcy(player: Player&, creditor: Player*,
                      context: GameContext&)
  - transferAssetsToPlayer(from: Player&, to: Player&)
  - returnAssetsToBank(player: Player&, context: GameContext&)
  - sellPropertyToBank(player: Player&, property: PropertyTile&): Money
  - mortgageProperty(player: Player&, property: PropertyTile&): Money
}
```

#### `FestivalManager`
```
class FestivalManager {
  - activeEffects: map<PropertyTile*, FestivalEffect>

  + applyFestival(player: Player&, property: PropertyTile&): FestivalResult
  + tickPlayerEffects(player: Player&)  // Called each turn for that player
  + getMultiplier(property: PropertyTile*): int
  + getDuration(property: PropertyTile*): int
  + hasActiveEffect(property: PropertyTile*): bool
}
```

```
struct FestivalEffect {
  Player* owner
  int multiplier         // 2, 4, or 8
  int turnsRemaining     // 1-3
  int timesApplied       // 1-3 (max 3 doublings)
}
```

#### `TransactionLogger`
```
class TransactionLogger {
  - entries: vector<LogEntry>

  + log(turn: int, username: string, actionType: string, detail: string)
  + getFullLog(): vector<LogEntry>
  + getLastN(n: int): vector<LogEntry>
  + getLogCount(): int
  + clearLog()
}
```

```
struct LogEntry {
  int turn
  string username
  string actionType
  string detail
}
```

### Exceptions

```
class NimonopoliException : public std::exception { ... }

class InsufficientFundsException : public NimonopoliException {
  - required: Money
  - available: Money
}

class InvalidCommandException : public NimonopoliException { ... }

class InvalidPropertyException : public NimonopoliException { ... }

class CardSlotFullException : public NimonopoliException {
  - maxSlots: int
}

class InvalidBidException : public NimonopoliException {
  - bidAmount: int
  - reason: string
}

class SaveLoadException : public NimonopoliException { ... }

class InvalidBoardConfigException : public NimonopoliException { ... }
```

### Enums

```
enum class TileType { STREET, RAILROAD, UTILITY, CHANCE, COMMUNITY_CHEST,
                      FESTIVAL, TAX_PPH, TAX_PBM, GO, JAIL,
                      FREE_PARKING, GO_TO_JAIL }

enum class PropertyStatus { BANK, OWNED, MORTGAGED }

enum class PlayerStatus { ACTIVE, BANKRUPT, JAILED }

enum class Color { BROWN, LIGHT_BLUE, PINK, ORANGE, RED, YELLOW,
                   GREEN, DARK_BLUE, DEFAULT, GRAY }

enum class SkillCardType { MOVE, DISCOUNT, SHIELD, TELEPORT, LASSO, DEMOLITION }

enum class LandingAction { BUY_PROMPT, PAY_RENT, PAY_TAX, DRAW_CHANCE,
                           DRAW_COMMUNITY, FESTIVAL_PROMPT, GO_TO_JAIL,
                           NOTHING, AUTO_ACQUIRE, BANKRUPT_CHECK }

enum class TaxChoice { FLAT, PERCENTAGE }
```

---

## Layer 3: Data Access Layer

### `ConfigLoader`
Reads all config files into `GameConfig`.

```
class ConfigLoader {
  + static loadGameConfig(configDir: string): GameConfig
  - static loadProperties(filepath: string): map<string, PropertyConfig>
  - static loadRailroadRents(filepath: string): map<int, int>
  - static loadUtilityMultipliers(filepath: string): map<int, int>
  - static loadTaxConfig(filepath: string): TaxConfig
  - static loadSpecialConfig(filepath: string): SpecialConfig
  - static loadMiscConfig(filepath: string): MiscConfig
  - static loadBoardLayout(filepath: string): vector<TileConfig>  // Dynamic board
}
```

### `SaveManager`
Handles save/load serialization.

```
class SaveManager {
  + static saveGame(filepath: string, state: GameState): bool
  + static loadGame(filepath: string): GameState
  - static serializePlayerState(player: PlayerState): string
  - static deserializePlayerState(data: string): PlayerState
  - static serializePropertyState(prop: PropertyState): string
  - static deserializePropertyState(data: string): PropertyState
  - static serializeDeckState(deck: DeckState): string
  - static deserializeDeckState(data: string): DeckState
  - static serializeLog(entries: vector<LogEntry>): string
  - static deserializeLog(data: string): vector<LogEntry>
}
```

### `BoardFactory`
Constructs the `Board` from config. Supports both static (40 tiles) and dynamic layouts.

```
class BoardFactory {
  + static createBoard(config: GameConfig): Board*
  + static createDynamicBoard(layout: vector<TileConfig>,
                              propertyConfigs: map<string, PropertyConfig>): Board*
  - static createTile(tileConfig: TileConfig,
                      propertyConfigs: map<string, PropertyConfig>): Tile*
  - static buildColorGroups(board: Board*): map<Color, ColorGroup*>
  - static validateBoardLayout(layout: vector<TileConfig>): bool
}
```

---

## Design Pattern Summary

| Pattern | Where Applied |
|---------|---------------|
| **Strategy** | `PlayerController` (Human vs COM) |
| **Abstract Factory** | `BoardFactory` (static vs dynamic board) |
| **Template Method** | `Tile::onLand()` polymorphic dispatch |
| **Observer-like** | `IGameView` notified by Game Logic |
| **Singleton-ish** | `TransactionLogger` (single instance owned by `GameEngine`) |
| **Generic/Template** | `CardDeck<T>` for all card types |
| **Command** | Each player command maps to a handler in `GameEngine` |
| **State** | `PlayerStatus`, `PropertyStatus` drive behavior branching |
| **Composition** | `GameEngine` composes `Board`, `TurnManager`, managers, etc. |

---

## OOP Requirement Mapping

| Requirement | Implementation |
|-------------|----------------|
| **Inheritance & Polymorphism** | `Tile` hierarchy, `Card` hierarchy, `PlayerController` hierarchy |
| **Exception Handling** | Custom exceptions for funds, bids, cards, saves |
| **Function Overloading** | Multiple constructors for tiles, cards |
| **Operator Overloading** | `Money` class (`+`, `+=`, `-`, `-=`, `<`, `>`, `==`), `Player` (`<`, `>`) |
| **Abstract Class & Virtual Function** | `Tile`, `PropertyTile`, `ActionCard`, `SkillCard`, `PlayerController`, `IGameView`, `IGameInput` |
| **Generic Class** | `CardDeck<T>` |
| **STL — Vector** | Players, tiles, cards, properties, log entries everywhere |
| **STL — Map** | `tilesByCode`, config lookups, `festivalEffects`, color groups |
| **Layered Architecture** | 3 strict layers with interface boundaries |
| **SRP (Single Responsibility)** | Each class has one job: `AuctionManager` only auctions, `FestivalManager` only festivals, etc. |
| **LSP (Liskov Substitution)** | Any `Tile*` can `onLand()`, any `SkillCard*` can `activate()`, any `PlayerController*` can decide |

---

## Bonus Coverage

| Bonus | Enabled By |
|-------|------------|
| **GUI** | `IGameView` / `IGameInput` abstract interfaces — `GUIView` / `GUIInput` plug in without changing game logic |
| **COM** | `PlayerController` abstract — `ComputerController` makes decisions autonomously |
| **Dynamic Board** | `BoardFactory::createDynamicBoard()` reads layout from config file, `Board` size is variable (20-60), all game logic uses `board.getSize()` not hardcoded 40 |
| **Creativity** | Architecture is extensible — new tile types, card types, or features slot in via inheritance |

---

## Key Data Flow Examples

### Player Lands on Street (Owned by Another)

```
GameEngine::executeTurn()
  → Dice::roll()
  → Player::setPosition(newPos)
  → Board::getTile(newPos) → StreetTile
  → StreetTile::onLand(player, context) → LandingResult{PAY_RENT, amount}
  → StreetTile::calculateRent()
      → checks monopoly, building level, festival multiplier
  → Bank::transferBetweenPlayers(player, owner, rent)
      → Player::deductMoney() [uses Money operator-=]
      → Player::addMoney()    [uses Money operator+=]
  → IGameView::showRentPayment(rentInfo)
  → TransactionLogger::log(...)

  If player can't afford:
  → BankruptcyManager::checkBankruptcy(player, rent, owner)
      → calculateMaxLiquidation()
      → if sufficient: executeLiquidation()
          → IGameView::showLiquidationPanel()
          → PlayerController::decideLiquidation()
      → if insufficient: executeBankruptcy()
          → transferAssetsToPlayer() or returnAssetsToBank()
          → if returnToBank: AuctionManager::startAuction() per property
```

### COM Player Turn

```
GameEngine::executeTurn(player)
  → player.getController() → ComputerController
  → ComputerController::chooseCommand(gameState)
      → evaluates: should I use a skill card? build? mortgage?
      → returns "LEMPAR_DADU"
  → Dice::roll()
  → ... landing logic same as human ...
  → if buy prompt: ComputerController::decideBuyProperty()
      → evaluates: cash reserves, monopoly potential, property value
      → returns true/false
  → IGameView::showMessage("COM buys Jakarta")
```

---

## File Structure (Recommended)

```
project-root/
├── config/                          # Configuration files
│   ├── property.txt
│   ├── railroad.txt
│   ├── utility.txt
│   ├── tax.txt
│   ├── special.txt
│   ├── misc.txt
│   └── board_layout.txt             # Dynamic board bonus
│
├── data/                            # DATA ACCESS LAYER (.cpp)
│   ├── ConfigLoader.cpp
│   ├── SaveManager.cpp
│   └── BoardFactory.cpp
│
├── include/                         # ALL HEADER FILES (.hpp)
│   ├── ui/
│   │   ├── IGameView.hpp
│   │   ├── IGameInput.hpp
│   │   ├── CLIView.hpp
│   │   ├── CLIInput.hpp
│   │   ├── GUIView.hpp              # Bonus: GUI
│   │   └── GUIInput.hpp             # Bonus: GUI
│   ├── core/
│   │   ├── GameEngine.hpp
│   │   ├── Bank.hpp
│   │   ├── TurnManager.hpp
│   │   ├── AuctionManager.hpp
│   │   ├── BankruptcyManager.hpp
│   │   ├── FestivalManager.hpp
│   │   └── CardSystem.hpp
│   ├── models/
│   │   ├── Money.hpp
│   │   ├── Player.hpp
│   │   ├── Board.hpp
│   │   ├── Dice.hpp
│   │   ├── GameState.hpp
│   │   ├── GameConfig.hpp
│   │   ├── ColorGroup.hpp
│   │   ├── TransactionLogger.hpp
│   │   ├── tiles/
│   │   │   ├── Tile.hpp
│   │   │   ├── PropertyTile.hpp
│   │   │   ├── StreetTile.hpp
│   │   │   ├── RailroadTile.hpp
│   │   │   ├── UtilityTile.hpp
│   │   │   ├── ChanceTile.hpp
│   │   │   ├── CommunityChestTile.hpp
│   │   │   ├── FestivalTile.hpp
│   │   │   ├── TaxTile.hpp
│   │   │   ├── GoTile.hpp
│   │   │   ├── JailTile.hpp
│   │   │   ├── FreeParkingTile.hpp
│   │   │   └── GoToJailTile.hpp
│   │   └── cards/
│   │       ├── Card.hpp
│   │       ├── CardDeck.hpp          # Template class (header-only)
│   │       ├── ActionCard.hpp
│   │       ├── ChanceCard.hpp
│   │       ├── CommunityChestCard.hpp
│   │       └── SkillCard.hpp
│   ├── controllers/
│   │   ├── PlayerController.hpp
│   │   ├── HumanController.hpp
│   │   └── ComputerController.hpp    # Bonus: COM
│   ├── data/
│   │   ├── ConfigLoader.hpp
│   │   ├── SaveManager.hpp
│   │   └── BoardFactory.hpp
│   └── utils/
│       ├── Enums.hpp
│       ├── Structs.hpp               # LandingResult, GameContext, etc.
│       └── Exceptions.hpp
│
├── src/                             # ALL SOURCE FILES (.cpp)
│   ├── main.cpp                     # Entry point
│   ├── core/                        # GAME LOGIC LAYER
│   │   ├── GameEngine.cpp
│   │   ├── Bank.cpp
│   │   ├── TurnManager.cpp
│   │   ├── AuctionManager.cpp
│   │   ├── BankruptcyManager.cpp
│   │   ├── FestivalManager.cpp
│   │   └── CardSystem.cpp
│   ├── models/                      # ENTITY / DATA MODEL CLASSES
│   │   ├── Money.cpp
│   │   ├── Player.cpp
│   │   ├── Board.cpp
│   │   ├── Dice.cpp
│   │   ├── ColorGroup.cpp
│   │   ├── TransactionLogger.cpp
│   │   ├── tiles/
│   │   │   ├── Tile.cpp
│   │   │   ├── PropertyTile.cpp
│   │   │   ├── StreetTile.cpp
│   │   │   ├── RailroadTile.cpp
│   │   │   ├── UtilityTile.cpp
│   │   │   ├── ChanceTile.cpp
│   │   │   ├── CommunityChestTile.cpp
│   │   │   ├── FestivalTile.cpp
│   │   │   ├── TaxTile.cpp
│   │   │   ├── GoTile.cpp
│   │   │   ├── JailTile.cpp
│   │   │   ├── FreeParkingTile.cpp
│   │   │   └── GoToJailTile.cpp
│   │   └── cards/
│   │       ├── ActionCard.cpp
│   │       ├── ChanceCard.cpp        # 3 subclasses
│   │       ├── CommunityChestCard.cpp # 3 subclasses
│   │       └── SkillCard.cpp          # 6 subclasses
│   ├── views/                       # UI LAYER
│   │   ├── CLIView.cpp
│   │   ├── CLIInput.cpp
│   │   ├── GUIView.cpp               # Bonus: GUI
│   │   └── GUIInput.cpp              # Bonus: GUI
│   └── utils/
│       └── Exceptions.cpp
│
├── .gitignore
├── README.md
└── makefile
```

### Layer ↔ Folder Mapping

| Layer | Headers (`include/`) | Sources |
|-------|---------------------|---------|
| **UI Layer** | `include/ui/`, `include/controllers/` | `src/views/` |
| **Game Logic Layer** | `include/core/` | `src/core/` |
| **Data Models** | `include/models/` | `src/models/` |
| **Data Access Layer** | `include/data/` | `data/` |
| **Utilities** | `include/utils/` | `src/utils/` |

Note: `include/` holds ALL `.hpp` files, mirroring the source structure. `CardDeck.hpp` is header-only since it's a template class. Special tiles (Go, Jail, FreeParking, GoToJail) each get their own file for clean SRP.
