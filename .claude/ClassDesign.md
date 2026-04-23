# Nimonspoli — Class Design Reference

Merged class design reference, combining the formal laporan class specifications (attributes, methods, relations) with the architectural framing from `OOP.md`. Use this as the authoritative spec when implementing.

---

## Architecture Overview

**3-Layer Architecture:**
1. **UI Layer** — All input/output. Zero game logic.
2. **Game Logic Layer** — Core rules, state, calculations. Zero I/O.
3. **Data Access Layer** — File reading/writing. Zero game logic.

**7 Architectural Pillars:**
1. **Command Pattern** — Each user command is its own class (21 commands).
2. **Observer Pattern** — `GameEngine` publishes `GameEvent` via `EventBus`; views and logger subscribe.
3. **Effect System** — Polymorphic `Effect` list on `Player` replaces ad-hoc bool fields.
4. **Builder Pattern** — `GameBuilder` assembles the `GameEngine` step-by-step.
5. **Repository Pattern** — `IGameRepository` abstracts save/load storage.
6. **Interface Segregation** — Focused strategy interfaces aggregated by `PlayerController`.
7. **Immutable State View** — `GameStateView` gives read-only snapshots to controllers & views.

---

# Layer 1: UI Layer

## `IGameView` (abstract)

**Attributes:** (none — state managed by concrete implementations)

**Methods:**
- `showBoard(boardState): void` — pure virtual
- `showDiceResult(d1, d2, playerName): void` — pure virtual
- `showPlayerLanding(playerName, tileName): void` — pure virtual
- `showPropertyCard(propertyInfo): void` — pure virtual
- `showPlayerProperties(list): void` — pure virtual
- `showBuyPrompt(propertyInfo, playerMoney): void` — pure virtual
- `showRentPayment(rentInfo): void` — pure virtual
- `showTaxPrompt(taxInfo): void` — pure virtual
- `showAuctionState(auctionState): void` — pure virtual
- `showFestivalPrompt(ownedProperties): void` — pure virtual
- `showBankruptcy(bankruptcyInfo): void` — pure virtual
- `showLiquidationPanel(liquidationState): void` — pure virtual
- `showCardDrawn(cardInfo): void` — pure virtual
- `showSkillCardHand(cards): void` — pure virtual
- `showTransactionLog(entries): void` — pure virtual
- `showWinner(winInfo): void` — pure virtual
- `showJailStatus(jailInfo): void` — pure virtual
- `showMessage(message): void` — pure virtual
- `showBuildMenu(buildMenuState): void` — pure virtual
- `showMortgageMenu(mortgageMenuState): void` — pure virtual
- `showRedeemMenu(redeemMenuState): void` — pure virtual
- `showDropCardPrompt(cards): void` — pure virtual
- `showSaveLoadStatus(message): void` — pure virtual
- `showTurnInfo(playerName, turnNum, maxTurn): void` — pure virtual

**Concrete implementations:** `CLIView`, `GUIView` (bonus).

---

## `IGameInput` (abstract)

**Attributes:** (none)

**Methods:**
- `getMenuChoice(options): int` — pure virtual
- `getCommand(): string` — pure virtual
- `getYesNo(prompt): bool` — pure virtual
- `getNumberInRange(prompt, min, max): int` — pure virtual
- `getString(prompt): string` — pure virtual
- `getManualDice(): pair<int,int>` — pure virtual
- `getAuctionAction(playerName, currentBid, playerMoney): AuctionAction` — pure virtual
- `getTaxChoice(): TaxChoice` — pure virtual
- `getLiquidationChoice(numOptions): int` — pure virtual
- `getSkillCardChoice(numCards): int` — pure virtual
- `getPropertyCodeInput(prompt): string` — pure virtual

**Concrete implementations:** `CLIInput`, `GUIInput` (bonus).

---

## `IEventListener` (abstract)

**Attributes:** (none)

**Methods:**
- `onEvent(event: GameEvent&): void` — pure virtual

---

## `PlayerController` (abstract)

**Attributes:** (none — abstract contract only)

**Methods:**
- `chooseCommand(state: GameStateView&): string` — pure virtual
- `decideBuyProperty(info, money): bool` — pure virtual
- `decideAuction(currentBid, money): AuctionAction` — pure virtual
- `decideTax(flatAmount, percentAmount): TaxChoice` — pure virtual
- `decideSkillCard(cards): int` — pure virtual
- `decideFestivalProperty(props): string` — pure virtual
- `decideBuild(state): int` — pure virtual
- `decideLiquidation(state): int` — pure virtual
- `decideDropCard(cards): int` — pure virtual
- `decideJailPay(): bool` — pure virtual
- `decideTeleportTarget(): string` — pure virtual
- `decideLassoTarget(players): string` — pure virtual
- `decideDemolitionTarget(properties): string` — pure virtual

---

## `HumanController : PlayerController`

**Attributes:**
- `input: IGameInput*`

**Methods:** (all delegate to `input`)
- `chooseCommand(state): string` — override
- `decideBuyProperty(info, money): bool` — override
- `decideAuction(currentBid, money): AuctionAction` — override
- `decideTax(flat, percent): TaxChoice` — override
- `decideSkillCard(cards): int` — override
- `decideFestivalProperty(props): string` — override
- `decideBuild(state): int` — override
- `decideLiquidation(state): int` — override
- `decideDropCard(cards): int` — override
- `decideJailPay(): bool` — override
- `decideTeleportTarget(): string` — override
- `decideLassoTarget(players): string` — override
- `decideDemolitionTarget(properties): string` — override

---

## `ComputerController : PlayerController`

**Attributes:**
- `reserveThreshold: int`

**Methods:** (greedy heuristics over `GameStateView`)
- `chooseCommand(state): string` — override
- `decideBuyProperty(info, money): bool` — override
- `decideAuction(currentBid, money): AuctionAction` — override
- `decideTax(flatAmount, percentAmount): TaxChoice` — override
- `decideSkillCard(cards): int` — override
- `decideFestivalProperty(props): string` — override
- `decideBuild(state): int` — override
- `decideLiquidation(state): int` — override
- `decideDropCard(cards): int` — override
- `decideJailPay(): bool` — override
- `decideTeleportTarget(): string` — override
- `decideLassoTarget(players): string` — override
- `decideDemolitionTarget(properties): string` — override

---

# Layer 2: Game Logic Layer

## `GameEngine`

**Attributes:**
- `board: Board*`
- `players: vector<Player*>`
- `turnManager: TurnManager`
- `auctionManager: AuctionManager`
- `bankruptcyManager: BankruptcyManager`
- `festivalManager: FestivalManager`
- `transactionLogger: TransactionLogger`
- `diceManager: Dice`
- `cardSystem: CardSystem`
- `bank: Bank`
- `gameConfig: GameConfig`
- `view: IGameView*`
- `gameState: GameState`
- `eventBus: EventBus*`
- `commandRegistry: CommandRegistry*`
- `repository: IGameRepository*`

**Methods:**
- `initialize(config, players): void`
- `runGameLoop(): void`
- `processCommand(command: string): void`
- `isGameOver(): bool`
- `getWinner(): vector<Player*>`
- `executeTurn(player: Player*): void` — private
- `handleLanding(player: Player*, tile: Tile*): void` — private
- `handleDoubles(player: Player*, n: int): void` — private
- `checkPassGo(player: Player*, oldPos: int, newPos: int): void` — private
- `processSkillCardPhase(player: Player*): void` — private
- `processDrawCardPhase(player: Player*): void` — private

---

## `GameBuilder`

**Attributes:**
- `config: GameConfig`
- `board: Board*`
- `players: vector<Player*>`
- `cardSystem: CardSystem*`
- `eventBus: EventBus*`
- `view: IGameView*`

**Methods:**
- `loadConfig(configDir: string): GameBuilder&`
- `buildBoard(): GameBuilder&`
- `buildCardDecks(): GameBuilder&`
- `addHumanPlayer(name: string): GameBuilder&`
- `addComputerPlayer(name: string, aiLevel: AILevel): GameBuilder&`
- `setView(view: IGameView*): GameBuilder&`
- `randomizeTurnOrder(): GameBuilder&`
- `loadFromSave(filepath: string): GameBuilder&`
- `build(): GameEngine*`

---

## `Bank`

**Attributes:** (none — pure method provider)

**Methods:**
- `payPlayer(player: Player&, amount: Money, reason: string): void`
- `collectFromPlayer(player: Player&, amount: Money, reason: string): void`
- `transferBetweenPlayers(from: Player&, to: Player&, amount: Money, reason: string): void`

---

## `TurnManager`

**Attributes:**
- `turnOrder: vector<Player*>`
- `currentIndex: int`
- `currentTurn: int`
- `maxTurn: int`

**Methods:**
- `getCurrentPlayer(): Player*`
- `advanceToNextPlayer(): void`
- `removePlayer(player: Player*): void`
- `getCurrentTurn(): int`
- `incrementTurn(): void`
- `isMaxTurnReached(): bool`
- `getActivePlayers(): vector<Player*>`
- `getActivePlayerCount(): int`
- `getTurnOrder(): vector<Player*>`
- `setTurnOrder(order: vector<Player*>): void`
- `getPlayerIndex(player: Player*): int`
- `getNextPlayerAfter(player: Player*): Player*`

---

## `AuctionManager`

**Attributes:**
- `currentBid: int`
- `highestBidder: Player*`
- `consecutivePasses: int`
- `participants: vector<Player*>`

**Methods:**
- `startAuction(property, players, triggerPlayer, context): AuctionResult`
- `runAuctionLoop(view: IGameView*): AuctionResult` — private
- `getNextBidder(): Player*` — private
- `isAuctionOver(activeCount: int): bool` — private

---

## `BankruptcyManager`

**Attributes:** (none — all data comes from parameters)

**Methods:**
- `checkBankruptcy(player, obligation, creditor, context): BankruptcyResult`
- `calculateMaxLiquidation(player: Player&): Money`
- `executeLiquidation(player, obligation, creditor, context): void`
- `executeBankruptcy(player, creditor, context): void`
- `transferAssetsToPlayer(from: Player&, to: Player&): void` — private
- `returnAssetsToBank(player: Player&, context): void` — private
- `sellPropertyToBank(player: Player&, property: PropertyTile&): Money` — private
- `mortgageProperty(player: Player&, property: PropertyTile&): Money` — private

---

## `FestivalManager`

**Attributes:**
- `activeEffects: map<PropertyTile*, FestivalEffect>`

**Methods:**
- `applyFestival(player: Player&, property: PropertyTile&): FestivalResult`
- `tickPlayerEffects(player: Player&): void`
- `getMultiplier(property: PropertyTile*): int`
- `getDuration(property: PropertyTile*): int`
- `hasActiveEffect(property: PropertyTile*): bool`

---

## `CardSystem`

**Attributes:**
- `chanceDeck: CardDeck<ChanceCard>`
- `communityChestDeck: CardDeck<CommunityChestCard>`
- `skillDeck: CardDeck<SkillCard>`

**Methods:**
- `drawChance(): ChanceCard*`
- `drawCommunityChest(): CommunityChestCard*`
- `drawSkill(): SkillCard*`
- `discardSkill(card: SkillCard*): void`
- `initializeDecks(): void`
- `getSkillDeck(): CardDeck<SkillCard>&`

---

## `TransactionLogger : IEventListener`

**Attributes:**
- `entries: vector<LogEntry>`

**Methods:**
- `TransactionLogger(): constructor`
- `log(turn: int, username: string, actionType: string, detail: string): void`
- `logEntry(entry: LogEntry): void` — overload for load
- `onEvent(event: GameEvent&): void` — override
- `getFullLog(): vector<LogEntry>`
- `getLastN(n: int): vector<LogEntry>`
- `getLogCount(): int`
- `clearLog(): void`
- `serializeForSave(): vector<LogEntry>`
- `loadFromSave(entries: vector<LogEntry>): void`
- `formatEntry(entry: LogEntry): string` — private helper

---

## `Money`

**Attributes:**
- `amount: int`

**Methods:**
- `Money(amount: int): constructor`
- `getAmount(): int`
- `setAmount(value: int): void`
- `isZero(): bool`
- `isPositive(): bool`
- `operator+(other: Money): Money`
- `operator-(other: Money): Money`
- `operator+=(other: Money): Money&`
- `operator-=(other: Money): Money&`
- `operator<(other: Money): bool`
- `operator>(other: Money): bool`
- `operator<=(other: Money): bool`
- `operator>=(other: Money): bool`
- `operator==(other: Money): bool`
- `operator!=(other: Money): bool`
- `static zero(): Money`
- `toString(): string`

---

## `Player`

**Attributes:**
- `username: string`
- `money: Money`
- `position: int`
- `status: PlayerStatus`
- `properties: vector<PropertyTile*>`
- `skillCards: vector<SkillCard*>`
- `activeEffects: vector<Effect*>`
- `controller: PlayerController*`
- `jailTurnsRemaining: int`
- `consecutiveDoubles: int`
- `hasUsedSkillCardThisTurn: bool`
- `hasRolledDiceThisTurn: bool`
- `turnCount: int`

**Methods:**
- `Player(username, startingMoney, controller): constructor`
- `getUsername(): string`
- `getMoney(): Money`
- `addMoney(amount: Money): void`
- `deductMoney(amount: Money): void`
- `canAfford(amount: Money): bool`
- `getPosition(): int`
- `setPosition(pos: int): void`
- `moveBy(steps: int, boardSize: int): void`
- `getStatus(): PlayerStatus`
- `setStatus(status: PlayerStatus): void`
- `isActive(): bool`
- `isBankrupt(): bool`
- `isJailed(): bool`
- `getProperties(): vector<PropertyTile*>`
- `addProperty(tile: PropertyTile*): void`
- `removeProperty(tile: PropertyTile*): void`
- `getPropertiesByColor(color: Color): vector<StreetTile*>`
- `hasMonopoly(color: Color): bool`
- `countRailroads(): int`
- `countUtilities(): int`
- `getSkillCards(): vector<SkillCard*>`
- `addSkillCard(card: SkillCard*): void` — throws `CardSlotFullException`
- `removeSkillCard(index: int): SkillCard*`
- `getActiveEffects(): vector<Effect*>`
- `addEffect(effect: Effect*): void`
- `tickEffects(): void`
- `applyOutgoingModifiers(amount: Money): Money`
- `applyIncomingModifiers(amount: Money, prop: PropertyTile*): Money`
- `isPaymentBlocked(): bool`
- `getController(): PlayerController*`
- `getTotalWealth(): Money`
- `resetTurnFlags(): void`
- `incrementJailTurn(): void`
- `operator<(other: Player&): bool`
- `operator>(other: Player&): bool`

---

## `Board`

**Attributes:**
- `tiles: vector<Tile*>`
- `tilesByCode: map<string, Tile*>`
- `size: int`
- `colorGroups: map<Color, ColorGroup*>`
- `goIndex: int`
- `jailIndex: int`

**Methods:**
- `Board(tiles, colorGroups): constructor`
- `~Board(): destructor`
- `getTile(index: int): Tile*`
- `getTileByCode(code: string): Tile*`
- `getSize(): int`
- `getGoIndex(): int`
- `getJailIndex(): int`
- `getColorGroup(color: Color): ColorGroup*`
- `findNearestStation(fromIndex: int): int`
- `getNextTileOfType(fromIndex: int, type: TileType): int`
- `getAllStreets(): vector<StreetTile*>`
- `getAllProperties(): vector<PropertyTile*>`
- `isValidIndex(index: int): bool`
- `wrapPosition(position: int): int`

---

## `ColorGroup`

**Attributes:**
- `color: Color`
- `streets: vector<StreetTile*>`

**Methods:**
- `ColorGroup(color: Color): constructor`
- `addStreet(street: StreetTile*): void`
- `getColor(): Color`
- `getStreets(): vector<StreetTile*>`
- `getStreetCount(): int`
- `isMonopolizedBy(player: Player*): bool`
- `getOwner(): Player*`
- `allHaveMinBuildings(level: int): bool`
- `getMaxBuildingDifference(): int`
- `canBuildOn(street: StreetTile*): bool`
- `getBuildableStreets(): vector<StreetTile*>`
- `hasAnyBuildings(): bool`
- `getTotalBuildingCount(): int`
- `getTotalBuildingValue(): Money`
- `getHalfBuildingValue(): Money`
- `sellAllBuildings(): Money`

---

## `Dice`

**Attributes:**
- `die1: int`
- `die2: int`
- `isManual: bool`

**Methods:**
- `roll(): pair<int,int>`
- `setManual(d1: int, d2: int): void`
- `getDie1(): int`
- `getDie2(): int`
- `getTotal(): int`
- `isDouble(): bool`

---

## `GameState`

**Attributes:**
- `currentTurn: int`
- `maxTurn: int`
- `players: vector<Player*>`
- `turnOrder: vector<Player*>`
- `activePlayerIndex: int`
- `propertyStates: vector<PropertyTile*>`
- `skillDeckState: CardDeck<SkillCard>*`
- `logEntries: vector<LogEntry>`
- `hasRolledDice: bool`
- `hasUsedSkillCard: bool`
- `consecutiveDoubles: int`

**Methods:**
- `GameState(): constructor`
- `getCurrentTurn(): int`
- `setCurrentTurn(turn: int): void`
- `getMaxTurn(): int`
- `getPlayers(): vector<Player*>`
- `getActivePlayer(): Player*`
- `getActivePlayerIndex(): int`
- `setActivePlayerIndex(idx: int): void`
- `getTurnOrder(): vector<Player*>`
- `setTurnOrder(order: vector<Player*>): void`
- `getLogEntries(): vector<LogEntry>`
- `setLogEntries(entries: vector<LogEntry>): void`
- `getHasRolledDice(): bool`
- `setHasRolledDice(value: bool): void`
- `getHasUsedSkillCard(): bool`
- `setHasUsedSkillCard(value: bool): void`
- `getConsecutiveDoubles(): int`
- `incrementConsecutiveDoubles(): void`
- `resetConsecutiveDoubles(): void`
- `resetTurnFlags(): void`
- `canSave(): bool`
- `toView(): GameStateView`

---

## `GameStateView` (immutable)

**Attributes:** (all `const`)
- `currentTurn: const int`
- `maxTurn: const int`
- `activePlayerName: const string`
- `activePlayerIndex: const int`
- `playerViews: const vector<PlayerView>`
- `propertyViews: const vector<PropertyView>`
- `boardSize: const int`
- `hasRolledDice: const bool`
- `hasUsedSkillCard: const bool`

**Methods:**
- `GameStateView(state: GameState&): constructor`
- `getCurrentTurn(): int`
- `getMaxTurn(): int`
- `getActivePlayerName(): string`
- `getActivePlayerIndex(): int`
- `getPlayers(): vector<PlayerView>`
- `getMyPlayer(username: string): PlayerView`
- `getOpponents(username: string): vector<PlayerView>`
- `getProperties(): vector<PropertyView>`
- `getPropertyByCode(code: string): PropertyView`
- `getOwnedPropertiesOf(username: string): vector<PropertyView>`
- `getBoardSize(): int`
- `getHasRolledDice(): bool`
- `getHasUsedSkillCard(): bool`

---

## `GameConfig`

**Attributes:**
- `properties: map<string, PropertyConfig>`
- `railroadRents: map<int, int>`
- `utilityMultipliers: map<int, int>`
- `pphFlat: int`
- `pphPercentage: int`
- `pbmFlat: int`
- `goSalary: int`
- `jailFine: int`
- `maxTurn: int`
- `startingBalance: int`
- `boardLayout: vector<TileConfig>`
- `configDir: string`

**Methods:**
- `GameConfig(): constructor`
- `getPropertyConfig(code: string): PropertyConfig`
- `getAllPropertyConfigs(): map<string, PropertyConfig>`
- `getRailroadRent(count: int): int`
- `getUtilityMultiplier(count: int): int`
- `getPphFlat(): int`
- `getPphPercentage(): int`
- `getPbmFlat(): int`
- `getGoSalary(): int`
- `getJailFine(): int`
- `getMaxTurn(): int`
- `getStartingBalance(): int`
- `getBoardLayout(): vector<TileConfig>`
- `getBoardSize(): int`
- `isBankruptcyMode(): bool`
- `isDynamicBoard(): bool`
- `validate(): bool` — throws `InvalidBoardConfigException`
- `setProperties(props): void`
- `setRailroadRents(rents): void`
- `setUtilityMultipliers(mults): void`
- `setTaxConfig(pphFlat, pphPct, pbmFlat): void`
- `setSpecialConfig(goSalary, jailFine): void`
- `setMiscConfig(maxTurn, startBalance): void`
- `setBoardLayout(layout): void`

---

# Tile Hierarchy

## `Tile` (abstract)

**Attributes:**
- `index: int`
- `code: string`
- `name: string`
- `type: TileType`

**Methods:**
- `Tile(index, code, name, type): protected constructor`
- `virtual ~Tile(): destructor`
- `getIndex(): int`
- `getCode(): string`
- `getName(): string`
- `getType(): TileType`
- `onLand(player: Player&, context: GameContext&): LandingResult` — pure virtual
- `onPass(player: Player&, context: GameContext&): void` — virtual, default empty
- `getDescription(): string` — virtual, default returns name

---

## `PropertyTile : Tile` (abstract)

**Attributes:**
- `owner: Player*`
- `status: PropertyStatus`
- `purchasePrice: Money`
- `mortgageValue: Money`
- `festivalMultiplier: int`
- `festivalDuration: int`
- `festivalTimesApplied: int`

**Methods:**
- `PropertyTile(...): protected constructor`
- `getOwner(): Player*`
- `setOwner(player: Player*): void`
- `getStatus(): PropertyStatus`
- `setStatus(status: PropertyStatus): void`
- `isOwned(): bool`
- `isMortgaged(): bool`
- `isAvailable(): bool`
- `getPurchasePrice(): Money`
- `getMortgageValue(): Money`
- `calculateRent(context: GameContext&): Money` — pure virtual
- `getSellValue(): Money` — virtual, default = `purchasePrice`
- `applyFestival(): bool`
- `tickFestival(): void`
- `isFestivalActive(): bool`
- `getFestivalMultiplier(): int`
- `onLand(player, context): LandingResult` — override

---

## `StreetTile : PropertyTile`

**Attributes:**
- `colorGroup: ColorGroup*`
- `buildingLevel: int` — 0 (empty), 1–4 (houses), 5 (hotel)
- `rentTable: vector<int>`
- `houseCost: Money`
- `hotelCost: Money`

**Methods:**
- `StreetTile(...): constructor`
- `getColorGroup(): ColorGroup*`
- `getBuildingLevel(): int`
- `getHouseCost(): Money`
- `getHotelCost(): Money`
- `getRentTable(): vector<int>`
- `isHotel(): bool`
- `isMaxedOut(): bool`
- `canBuild(): bool`
- `buildHouse(): void`
- `upgradeToHotel(): void`
- `sellOneBuilding(): Money`
- `calculateRent(context): Money` — override
- `getSellValue(): Money` — override (purchasePrice + half of building value)
- `onLand(player, context): LandingResult` — override

---

## `RailroadTile : PropertyTile`

**Attributes:** (none beyond inherited)

**Methods:**
- `RailroadTile(...): constructor`
- `calculateRent(context): Money` — override
- `onLand(player, context): LandingResult` — override (AUTO_ACQUIRE if BANK)

---

## `UtilityTile : PropertyTile`

**Attributes:** (none beyond inherited)

**Methods:**
- `UtilityTile(...): constructor`
- `calculateRent(context): Money` — override (diceTotal × multiplier)
- `onLand(player, context): LandingResult` — override

---

## `ActionTile : Tile` (abstract)

**Attributes:** (none beyond inherited)

**Methods:**
- `ActionTile(...): protected constructor`
- `onLand(player, context): LandingResult` — pure virtual

---

## `ChanceTile : ActionTile`

**Attributes:** (none)

**Methods:**
- `onLand(player, context): LandingResult` — override, returns `{DRAW_CHANCE}`

---

## `CommunityChestTile : ActionTile`

**Attributes:** (none)

**Methods:**
- `onLand(player, context): LandingResult` — override, returns `{DRAW_COMMUNITY}`

---

## `FestivalTile : ActionTile`

**Attributes:** (none)

**Methods:**
- `onLand(player, context): LandingResult` — override, returns `{FESTIVAL_PROMPT}`

---

## `TaxTile : ActionTile` (abstract)

**Attributes:**
- `baseAmount: int`

**Methods:**
- `TaxTile(...): protected constructor`
- `calculateTax(player: Player&, choice: TaxChoice): Money` — pure virtual
- `onLand(player, context): LandingResult` — override, returns `{PAY_TAX}`

---

## `PPHTile : TaxTile`

**Attributes:**
- `percentage: int`

**Methods:**
- `calculateTax(player, choice): Money` — override
- `getPercentage(): int`

---

## `PBMTile : TaxTile`

**Attributes:** (none)

**Methods:**
- `calculateTax(player, choice): Money` — override (always flat)

---

## `SpecialTile : Tile` (abstract)

**Attributes:** (none)

**Methods:**
- `SpecialTile(...): protected constructor`
- `onLand(player, context): LandingResult` — pure virtual

---

## `GoTile : SpecialTile`

**Attributes:** (none)

**Methods:**
- `onLand(player, context): LandingResult` — override, pays salary, returns `{NOTHING}`
- `onPass(player, context): void` — override, pays salary

---

## `JailTile : SpecialTile`

**Attributes:** (none)

**Methods:**
- `onLand(player, context): LandingResult` — override, returns `{NOTHING}`

---

## `FreeParkingTile : SpecialTile`

**Attributes:** (none)

**Methods:**
- `onLand(player, context): LandingResult` — override, returns `{NOTHING}`

---

## `GoToJailTile : SpecialTile`

**Attributes:** (none)

**Methods:**
- `onLand(player, context): LandingResult` — override, returns `{GO_TO_JAIL}`

---

# Card System

## `Card` (abstract)

**Attributes:**
- `name: string`
- `description: string`

**Methods:**
- `Card(name, description): constructor`
- `virtual ~Card(): destructor`
- `getName(): string`
- `getDescription(): string`
- `getType(): string` — pure virtual

---

## `ActionCard : Card` (abstract)

**Attributes:** (none beyond inherited)

**Methods:**
- `ActionCard(name, description): constructor`
- `virtual ~ActionCard(): destructor`
- `execute(player: Player&, ctx: GameContext&): CardResult` — pure virtual
- `getType(): string` — returns `"ActionCard"`

---

## `ChanceCard : ActionCard`

**Subclasses with their `execute()` overrides:**
- `ChanceGoToStation` — move to nearest station
- `ChanceMoveBack3` — move back 3 tiles
- `ChanceGoToJail` — go to jail

---

## `CommunityChestCard : ActionCard`

**Subclasses with their `execute()` overrides:**
- `CommunityBirthday` — receive M100 from each player
- `CommunityDoctorFee` — pay M700
- `CommunityElection` — pay M200 to each player

---

## `SkillCard : Card` (abstract)

**Attributes:** (none beyond inherited)

**Methods:**
- `SkillCard(name, description): constructor`
- `virtual ~SkillCard(): destructor`
- `activate(player: Player&, ctx: GameContext&): CardResult` — pure virtual
- `getCardType(): SkillCardType` — pure virtual
- `getSaveValue(): string` — pure virtual
- `getSaveDuration(): string` — pure virtual
- `getType(): string` — returns `"SkillCard"`

**Subclasses:**
- `MoveCard` — attribute: `steps: int`; moves player forward
- `DiscountCard` — attributes: `percentage: int`, `turnsLeft: int`; adds `DiscountEffect`
- `ShieldCard` — adds `ShieldEffect` (1 turn)
- `TeleportCard` — player chooses target tile
- `LassoCard` — player chooses opponent ahead to pull
- `DemolitionCard` — player chooses opponent property to destroy

---

## `CardDeck<T>` (generic)

**Attributes:**
- `drawCards: vector<T*>`
- `usedCards: vector<T*>`

**Methods:**
- `CardDeck(): constructor`
- `~CardDeck(): destructor`
- `addCard(card: T*): void`
- `shuffle(): void`
- `reshuffleUsed(): void`
- `draw(): T*`
- `discard(card: T*): void`
- `isEmpty(): bool`
- `getRemainingCount(): int`
- `getTotalCount(): int`
- `getDrawCards(): const vector<T*>&`
- `getUsedCards(): const vector<T*>&`
- `setDrawCards(cards): void`
- `setUsedCards(cards): void`

---

# Effect System

## `Effect` (abstract)

**Attributes:**
- `remainingTurns: int`

**Methods:**
- `Effect(remainingTurns: int): constructor`
- `virtual ~Effect(): destructor`
- `onStart(player: Player&): void` — virtual
- `onEnd(player: Player&): void` — virtual
- `modifyOutgoingPayment(amount: Money): Money` — virtual
- `modifyRent(amount: Money, prop: PropertyTile*): Money` — virtual
- `blockPayment(): bool` — virtual
- `isExpired(): bool`
- `getRemainingTurns(): int`
- `getDescription(): string` — virtual
- `getEffectType(): string` — virtual

**Subclasses:**
- `ShieldEffect` — `blockPayment()` = true, 1 turn
- `DiscountEffect` — modifies outgoing payment by percentage
- `FestivalEffect` — modifies rent received (multiplier 2/4/8), 3 turns

---

# Event System

## `GameEvent` (abstract)

**Attributes:** (none at base)

**Methods:**
- `virtual ~GameEvent(): destructor`
- `getType(): EventType` — pure virtual

**Concrete event subclasses:**
- `PlayerMovedEvent` — `{player, fromTile, toTile}`
- `MoneyChangedEvent` — `{player, oldAmount, newAmount, reason}`
- `PropertyAcquiredEvent` — `{player, property, price}`
- `RentPaidEvent` — `{payer, owner, amount, property}`
- `BuildingBuiltEvent`
- `MortgagedEvent`
- `RedeemedEvent`
- `FestivalActivatedEvent`
- `AuctionStartedEvent`
- `BidPlacedEvent`
- `CardDrawnEvent`
- `SkillCardUsedEvent`
- `EffectAppliedEvent`
- `BankruptcyEvent`
- `TurnStartedEvent`
- `TurnEndedEvent`
- `GameOverEvent`

---

## `EventBus`

**Attributes:**
- `listeners: map<EventType, vector<IEventListener*>>`

**Methods:**
- `subscribe(type: EventType, listener: IEventListener*): void`
- `publish(event: GameEvent&): void`

---

# Command System

## `Command` (abstract)

**Attributes:**
- `name: string`

**Methods:**
- `Command(name: string): constructor`
- `virtual ~Command(): destructor`
- `execute(engine: GameEngine&, player: Player&): void` — pure virtual
- `canExecute(state: GameStateView&): bool` — pure virtual
- `getName(): string` — virtual
- `getHelp(): string` — virtual

**Concrete subclasses (21 commands):**
`RollDiceCommand`, `SetDiceCommand`, `PrintBoardCommand`, `PrintDeedCommand`, `PrintPropertiesCommand`, `PrintLogCommand`, `BuildCommand`, `MortgageCommand`, `RedeemCommand`, `UseSkillCardCommand`, `SaveCommand`, `LoadCommand`, plus the remaining spec commands.

---

## `CommandRegistry`

**Attributes:**
- `commands: map<string, Command*>`

**Methods:**
- `registerCommand(cmd: Command*): void`
- `parseAndExecute(input: string, engine: GameEngine&, player: Player&): void`
- `getAvailableCommands(state: GameStateView&): vector<string>`

---

# Layer 3: Data Access Layer

## `IGameRepository` (abstract)

**Attributes:** (none)

**Methods:**
- `save(state: GameState&, identifier: string): bool` — pure virtual
- `load(identifier: string): GameState` — pure virtual
- `exists(identifier: string): bool` — pure virtual

**Concrete implementations:** `TextFileRepository`, `JsonRepository` (bonus), `LeaderboardRepository` (bonus).

---

## `ConfigLoader`

**Attributes:** (none — all methods static)

**Methods:**
- `static loadGameConfig(configDir: string): GameConfig`
- `static loadProperties(filepath: string): map<string, PropertyConfig>` — private
- `static loadRailroadRents(filepath: string): map<int,int>` — private
- `static loadUtilityMultipliers(filepath: string): map<int,int>` — private
- `static loadTaxConfig(filepath: string): TaxConfig` — private
- `static loadSpecialConfig(filepath: string): SpecialConfig` — private
- `static loadMiscConfig(filepath: string): MiscConfig` — private
- `static loadBoardLayout(filepath: string): vector<TileConfig>` — private

---

## `SaveManager`

**Attributes:** (none — all methods static)

**Methods:**
- `static saveGame(filepath: string, state: GameState): bool`
- `static loadGame(filepath: string): GameState`
- `static serializePlayerState(player: PlayerState): string` — private
- `static deserializePlayerState(data: string): PlayerState` — private
- `static serializePropertyState(prop: PropertyState): string` — private
- `static deserializePropertyState(data: string): PropertyState` — private
- `static serializeDeckState(deck: DeckState): string` — private
- `static deserializeDeckState(data: string): DeckState` — private
- `static serializeLog(entries: vector<LogEntry>): string` — private
- `static deserializeLog(data: string): vector<LogEntry>` — private

---

## `BoardFactory`

**Attributes:** (none — all methods static)

**Methods:**
- `static createBoard(config: GameConfig): Board*`
- `static createDynamicBoard(layout: vector<TileConfig>, propertyConfigs: map<string, PropertyConfig>): Board*`
- `static createTile(tileConfig, propertyConfigs): Tile*` — private
- `static buildColorGroups(board: Board*): map<Color, ColorGroup*>` — private
- `static validateBoardLayout(layout: vector<TileConfig>): bool` — private

---

# Enums

- `TileType` — `STREET, RAILROAD, UTILITY, CHANCE, COMMUNITY_CHEST, FESTIVAL, TAX_PPH, TAX_PBM, GO, JAIL, FREE_PARKING, GO_TO_JAIL`
- `PropertyStatus` — `BANK, OWNED, MORTGAGED`
- `PlayerStatus` — `ACTIVE, BANKRUPT, JAILED`
- `Color` — `BROWN, LIGHT_BLUE, PINK, ORANGE, RED, YELLOW, GREEN, DARK_BLUE, DEFAULT, GRAY`
- `SkillCardType` — `MOVE, DISCOUNT, SHIELD, TELEPORT, LASSO, DEMOLITION`
- `LandingAction` — `BUY_PROMPT, PAY_RENT, PAY_TAX, DRAW_CHANCE, DRAW_COMMUNITY, FESTIVAL_PROMPT, GO_TO_JAIL, NOTHING, AUTO_ACQUIRE, BANKRUPT_CHECK`
- `TaxChoice` — `FLAT, PERCENTAGE`
- `AuctionAction` — `BID, PASS`
- `AILevel` — `BASIC, SMART`
- `EventType` — one value per `GameEvent` subclass

---

# Exceptions

## `NimonopoliException : std::exception`

Base class for all game-specific exceptions.

## Subclasses

- `InsufficientFundsException` — attributes: `required: Money`, `available: Money`
- `InvalidCommandException`
- `InvalidPropertyException`
- `CardSlotFullException` — attribute: `maxSlots: int`
- `InvalidBidException` — attributes: `bidAmount: int`, `reason: string`
- `SaveLoadException`
- `InvalidBoardConfigException`

---

# Structs (passive data holders)

## `GameContext`
```
Bank& bank
FestivalManager& festivalManager
TransactionLogger& logger
GameConfig& config
Dice& dice
vector<Player*>& players
Board& board
int currentTurn
```

## `LandingResult`
```
LandingAction action
int amount
Player* creditor
PropertyTile* property
```

## `LogEntry`
```
int turn
string username
string actionType
string detail
```

## `AuctionResult`
```
Player* winner
int finalPrice
PropertyTile* property
```

## `FestivalEffect`
```
Player* owner
int multiplier      // 2, 4, or 8
int turnsRemaining  // 1-3
int timesApplied    // max 3
```

## `PlayerView` (read-only snapshot)
```
string username
Money money
int position
PlayerStatus status
int propertyCount
int skillCardCount
```

## `PropertyView` (read-only snapshot)
```
string code
string name
string ownerName    // empty if BANK
PropertyStatus status
int buildingLevel   // 0 for non-street
```

---

# Design Pattern Summary

| Pattern | Where Applied |
| --- | --- |
| Strategy | `PlayerController` (Human vs COM) |
| Abstract Factory | `BoardFactory` (static vs dynamic board) |
| Template Method | `Tile::onLand()` polymorphic dispatch |
| Observer | `EventBus` + `IEventListener` (`CLIView`, `GUIView`, `TransactionLogger`) |
| Command | `Command` hierarchy + `CommandRegistry` |
| Builder | `GameBuilder` |
| Repository | `IGameRepository` |
| Generic/Template | `CardDeck<T>` |
| State | `PlayerStatus`, `PropertyStatus` |
| Composition | `GameEngine` owns `Board`, managers, etc. |

---

# OOP Requirement Mapping

| Requirement | Implementation |
| --- | --- |
| Inheritance & Polymorphism | `Tile`, `Card`, `PlayerController`, `Effect`, `GameEvent` hierarchies |
| Exception Handling | `NimonopoliException` hierarchy |
| Function Overloading | Multiple constructors for tiles/cards; `TransactionLogger::log` / `logEntry` |
| Operator Overloading | `Money` (`+`, `+=`, `-`, `-=`, `<`, `>`, `<=`, `>=`, `==`, `!=`), `Player` (`<`, `>`) |
| Abstract Class & Virtual Function | `Tile`, `PropertyTile`, `TaxTile`, `SpecialTile`, `ActionTile`, `ActionCard`, `SkillCard`, `Effect`, `PlayerController`, `IGameView`, `IGameInput`, `IEventListener`, `IGameRepository`, `Command` |
| Generic Class | `CardDeck<T>` |
| STL — Vector | Players, tiles, cards, properties, log entries |
| STL — Map | `tilesByCode`, `colorGroups`, `activeEffects`, `listeners`, config lookups |
| Layered Architecture | 3 layers (UI / Game Logic / Data Access) with interface boundaries |
| SRP | Each manager has one job (Auction, Bankruptcy, Festival, Turn) |
| LSP | Any `Tile*`, `Card*`, `Effect*`, `PlayerController*` substitutable |

---

# File Structure

```
project-root/
├── config/                         # Config files
│   ├── property.txt
│   ├── railroad.txt
│   ├── utility.txt
│   ├── tax.txt
│   ├── special.txt
│   ├── misc.txt
│   └── board_layout.txt            # Dynamic board bonus
│
├── include/
│   ├── ui/
│   │   ├── IGameView.hpp
│   │   ├── IGameInput.hpp
│   │   ├── CLIView.hpp
│   │   ├── CLIInput.hpp
│   │   ├── GUIView.hpp             # Bonus
│   │   └── GUIInput.hpp            # Bonus
│   ├── core/
│   │   ├── GameEngine.hpp
│   │   ├── GameBuilder.hpp
│   │   ├── Bank.hpp
│   │   ├── TurnManager.hpp
│   │   ├── AuctionManager.hpp
│   │   ├── BankruptcyManager.hpp
│   │   ├── FestivalManager.hpp
│   │   ├── CardSystem.hpp
│   │   ├── EventBus.hpp
│   │   └── CommandRegistry.hpp
│   ├── models/
│   │   ├── Money.hpp
│   │   ├── Player.hpp
│   │   ├── Board.hpp
│   │   ├── Dice.hpp
│   │   ├── GameState.hpp
│   │   ├── GameStateView.hpp
│   │   ├── GameConfig.hpp
│   │   ├── ColorGroup.hpp
│   │   ├── TransactionLogger.hpp
│   │   ├── tiles/
│   │   │   ├── Tile.hpp
│   │   │   ├── PropertyTile.hpp
│   │   │   ├── StreetTile.hpp
│   │   │   ├── RailroadTile.hpp
│   │   │   ├── UtilityTile.hpp
│   │   │   ├── ActionTile.hpp
│   │   │   ├── ChanceTile.hpp
│   │   │   ├── CommunityChestTile.hpp
│   │   │   ├── FestivalTile.hpp
│   │   │   ├── TaxTile.hpp
│   │   │   ├── PPHTile.hpp
│   │   │   ├── PBMTile.hpp
│   │   │   ├── SpecialTile.hpp
│   │   │   ├── GoTile.hpp
│   │   │   ├── JailTile.hpp
│   │   │   ├── FreeParkingTile.hpp
│   │   │   └── GoToJailTile.hpp
│   │   ├── cards/
│   │   │   ├── Card.hpp
│   │   │   ├── CardDeck.hpp        # Template (header-only)
│   │   │   ├── ActionCard.hpp
│   │   │   ├── ChanceCard.hpp
│   │   │   ├── CommunityChestCard.hpp
│   │   │   └── SkillCard.hpp
│   │   ├── effects/
│   │   │   ├── Effect.hpp
│   │   │   ├── ShieldEffect.hpp
│   │   │   ├── DiscountEffect.hpp
│   │   │   └── FestivalEffect.hpp
│   │   └── events/
│   │       ├── GameEvent.hpp
│   │       └── IEventListener.hpp
│   ├── controllers/
│   │   ├── PlayerController.hpp
│   │   ├── HumanController.hpp
│   │   └── ComputerController.hpp  # Bonus: COM
│   ├── commands/
│   │   ├── Command.hpp
│   │   └── (one file per concrete command)
│   ├── data/
│   │   ├── IGameRepository.hpp
│   │   ├── TextFileRepository.hpp
│   │   ├── ConfigLoader.hpp
│   │   ├── SaveManager.hpp
│   │   └── BoardFactory.hpp
│   └── utils/
│       ├── Enums.hpp
│       ├── Structs.hpp
│       └── Exceptions.hpp
│
├── src/
│   ├── main.cpp
│   ├── core/
│   ├── models/
│   ├── views/
│   ├── controllers/
│   ├── commands/
│   ├── data/
│   └── utils/
│
├── .gitignore
├── README.md
└── makefile
```

| Layer | Headers | Sources |
| --- | --- | --- |
| UI Layer | `include/ui/`, `include/controllers/`, `include/commands/` | `src/views/`, `src/controllers/`, `src/commands/` |
| Game Logic Layer | `include/core/` | `src/core/` |
| Data Models | `include/models/` | `src/models/` |
| Data Access Layer | `include/data/` | `src/data/` |
| Utilities | `include/utils/` | `src/utils/` |
