# Task 1 Board + Tiles + Property Rules
Implement:

Models (board & grouping)

include/models/Board.hpp + src/models/Board.cpp
include/models/ColorGroup.hpp + src/models/ColorGroup.cpp
Tiles (SEMUA yang ada di tiles/)

include/models/tiles/Tile.hpp + src/models/tiles/Tile.cpp
include/models/tiles/PropertyTile.hpp + src/models/tiles/PropertyTile.cpp
include/models/tiles/StreetTile.hpp + src/models/tiles/StreetTile.cpp
include/models/tiles/RailroadTile.hpp + src/models/tiles/RailroadTile.cpp
include/models/tiles/UtilityTile.hpp + src/models/tiles/UtilityTile.cpp
include/models/tiles/ActionTile.hpp + src/models/tiles/ActionTile.cpp
include/models/tiles/ChanceTile.hpp + src/models/tiles/ChanceTile.cpp
include/models/tiles/CommunityChestTile.hpp + src/models/tiles/CommunityChestTile.cpp
include/models/tiles/FestivalTile.hpp + src/models/tiles/FestivalTile.cpp
include/models/tiles/TaxTile.hpp + src/models/tiles/TaxTile.cpp
include/models/tiles/PPHTile.hpp + src/models/tiles/PPHTile.cpp
include/models/tiles/PBMTile.hpp + src/models/tiles/PBMTile.cpp
include/models/tiles/SpecialTile.hpp + src/models/tiles/SpecialTile.cpp
include/models/tiles/GoTile.hpp + src/models/tiles/GoTile.cpp
include/models/tiles/JailTile.hpp + src/models/tiles/JailTile.cpp
include/models/tiles/FreeParkingTile.hpp + src/models/tiles/FreeParkingTile.cpp
include/models/tiles/GoToJailTile.hpp + src/models/tiles/GoToJailTile.cpp

# Task 2 Player + Money + Effects + Festival (+ Bank)
Implement:

Money & Player

include/models/Money.hpp + src/models/Money.cpp
include/models/Player.hpp + src/models/Player.cpp
Effects (SEMUA yang ada di effects/)

include/models/effects/Effect.hpp + src/models/effects/Effect.cpp (kalau base class perlu implement non-trivial; kalau murni abstract bisa header-only)
include/models/effects/ShieldEffect.hpp + src/models/effects/ShieldEffect.cpp
include/models/effects/DiscountEffect.hpp + src/models/effects/DiscountEffect.cpp
include/models/effects/FestivalEffect.hpp + src/models/effects/FestivalEffect.cpp
Festival & transaksi uang (core service)

include/core/FestivalManager.hpp + src/core/FestivalManager.cpp
include/core/Bank.hpp + src/core/Bank.cpp

# Task 3 Card System (Deck + semua card) + CardSystem
Implement:

Cards (SEMUA yang ada di cards/)

include/models/cards/Card.hpp + src/models/cards/Card.cpp (kalau ada method non-pure; kalau murni interface bisa minimal/inline)
include/models/cards/CardDeck.hpp (template header-only; biasanya tidak ada .cpp)
include/models/cards/ActionCard.hpp + src/models/cards/ActionCard.cpp
include/models/cards/ChanceCard.hpp + src/models/cards/ChanceCard.cpp
include/models/cards/CommunityChestCard.hpp + src/models/cards/CommunityChestCard.cpp
include/models/cards/SkillCard.hpp + src/models/cards/SkillCard.cpp
Core card orchestrator

include/core/CardSystem.hpp + src/core/CardSystem.cpp

# Task 4 UI + Input + Controllers (tanpa game logic)
Implement:

UI (sesuai dokumen)

include/ui/IGameView.hpp + src/views/IGameView.cpp (atau header-only interface; tergantung implementasi kamu)
include/ui/IGameInput.hpp + src/views/IGameInput.cpp (atau header-only interface)
include/ui/CLIView.hpp + src/views/CLIView.cpp
include/ui/CLIInput.hpp + src/views/CLIInput.cpp
include/ui/GUIView.hpp + src/views/GUIView.cpp (bonus)
include/ui/GUIInput.hpp + src/views/GUIInput.cpp (bonus)
Controllers

include/controllers/PlayerController.hpp + src/controllers/PlayerController.cpp (biasanya abstract; bisa tanpa .cpp)
include/controllers/HumanController.hpp + src/controllers/HumanController.cpp
include/controllers/ComputerController.hpp + src/controllers/ComputerController.cpp (bonus)
(Kalau repo kamu saat ini memakai views alih-alih include/ui/, tim UI yang menyesuaikan mapping foldernya.)

# Task 5 Game Orchestration + Commands + State + Event/Logging + Data Access
Implement:

Core game orchestration & managers

include/core/GameEngine.hpp + src/core/GameEngine.cpp
include/core/GameBuilder.hpp + src/core/GameBuilder.cpp
include/core/TurnManager.hpp + src/core/TurnManager.cpp
include/core/AuctionManager.hpp + src/core/AuctionManager.cpp
include/core/BankruptcyManager.hpp + src/core/BankruptcyManager.cpp
include/core/EventBus.hpp + src/core/EventBus.cpp
include/core/CommandRegistry.hpp + src/core/CommandRegistry.cpp
Game state & supporting models

include/models/GameState.hpp + src/models/GameState.cpp
include/models/GameStateView.hpp + src/models/GameStateView.cpp
include/models/Dice.hpp + src/models/Dice.cpp
include/models/GameConfig.hpp + src/models/GameConfig.cpp
include/models/TransactionLogger.hpp + src/models/TransactionLogger.cpp
Events (observer payload types)

include/models/events/GameEvent.hpp + src/models/events/GameEvent.cpp (kalau ada non-pure)
include/models/events/IEventListener.hpp (biasanya interface header-only; implementasinya ada di listener seperti view/logger)
Commands (Command Pattern)

include/commands/Command.hpp + src/commands/Command.cpp (base; seringnya abstract)
include/commands/(one file per concrete command) + src/commands/(matching .cpp)
minimal yang disebut dokumen: RollDiceCommand, SetDiceCommand, PrintBoardCommand, PrintDeedCommand, PrintPropertiesCommand, PrintLogCommand, BuildCommand, MortgageCommand, RedeemCommand, UseSkillCardCommand, SaveCommand, LoadCommand
plus “remaining spec commands” sampai total 21 command
Data access layer

include/data/IGameRepository.hpp (interface; biasanya header-only)
include/data/TextFileRepository.hpp + src/data/TextFileRepository.cpp
include/data/ConfigLoader.hpp + src/data/ConfigLoader.cpp
include/data/SaveManager.hpp + src/data/SaveManager.cpp
include/data/BoardFactory.hpp + src/data/BoardFactory.cpp
Entry point

main.cpp