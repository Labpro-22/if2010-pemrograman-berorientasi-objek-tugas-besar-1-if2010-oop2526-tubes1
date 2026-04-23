# NIMONSPOLY
## IF2010 Object Oriented Programming

This program is made for IF2010 Object Oriented Programming course @ Institut Teknologi Bandung.

## Meet the developers – [BCC] BurntCheesecake

Made Branenda Jordhy — 13524026  
Muhammad Nur Majiid — 13524028  
Jason Edward Salim — 13524034  
Bryan Pratama Putra Hendra — 13524067  
Athilla Zaidan Zidna Fann — 13524068  

## Requirements

- CMake (>= 3.16)
- C++17 compiler (`clang++`, `g++`, or MSVC)
- SFML 3.x (for GUI mode)

## How to Run

### 1. Enable SFML GUI (Recommended)

Requires SFML 3.x installed and discoverable by CMake.

**macOS (Homebrew):**
```bash
brew install sfml
```

**Build & Run:**
```bash
cmake -S . -B build -DNIMONSPOLY_ENABLE_SFML=ON
cmake --build build
./bin/game
```

### 2. CLI Mode Only (No GUI)

```bash
cmake -S . -B build
cmake --build build
./bin/game
```

### Clean

```bash
rm -rf build bin
```

## Implemented Features

### Game Logic
- Complete Monopoly board traversal and tile interactions
- Property purchase, rent, and mortgage system
- Building (house/hotel) upgrade system
- Festival system with rent multipliers
- Jail system with fine, roll doubles, and escape cards
- Chance and Community Chest cards with effects
- Auction system for unbought properties
- Bankruptcy and liquidation handling
- Save / Load game state
- Turn manager with dice rolling

### GUI (SFML)
- Full in-game board with globe background and title
- Pixel-perfect tile rendering (no gaps)
- Player token rendering with colors
- Left panel: player list, game log, round info
- Right panel: action buttons (Buy, Build, Mortgage, Card, Save, End), dice button, property mini-cards
- Dice roll animation and overlay
- Prompt overlays (buy, auction, input)
- Save/Load status toast

## Not Yet Implemented
- Sound effects and background music
- Settings menu (volume, fullscreen)
- Animated card draw effects
- Particle effects for festivals / wins
- Multiplayer over network
- AI bot players
