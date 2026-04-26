# NIMONSPOLY
## IF2010 Object Oriented Programming
NIMONSPOLY is a C++ Monopoly-inspired board game project developed with a strong Object-Oriented Programming approach to model the game’s core components in a modular and maintainable way, including players, boards, tiles, properties, cards, transactions, turn management, configuration handling, and save/load functionality. The game supports interactive gameplay through features such as property purchasing and auctions, rent payments, taxes, jail mechanics, Chance and Community Chest cards, skill cards, festival effects, bankruptcy handling, and winner calculation based on the final game state. In addition to the command-line interface, NIMONSPOLY includes a Raylib-based graphical user interface that visualizes the game board, player status, real-time action logs, integrated input prompts, dice animations, and direct interactions with tiles and owned properties. The project is designed for extensibility through clear class responsibility separation, inheritance and polymorphism in the tile and card systems, external configuration files, and support for flexible board layouts.

## Meet the developers – [BCC] BurntCheesecake 

Made Branenda Jordhy<br>
13524026<br>
[GitHub Account](https://github.com/ethj0r)<br>

Muhammad Nur Majiid<br>
13524028<br>
[GitHub Account](https://github.com/MAJIIDMN)<br>

Jason Edward Salim<br>
13524034<br>
[GitHub Account](https://github.com/jsndwrd)<br>

Bryan Pratama Putra Hendra<br>
13524067<br>
[GitHub Account](https://github.com/Bryannpph)<br>

Athilla Zaidan Zidna Fann<br>
13524068<br>
[GitHub Account](https://github.com/AthillaZaidan)<br>

## Directory

1. `config/`: game configuration files.
2. `data/`: runtime data (if needed).
3. `include/`: header files (`.hpp`).
4. `src/`: source files (`.cpp`).
5. `build/`: CMake build directory (generated).
6. `bin/`: executable output directory (generated).
7. `CMakeLists.txt`: CMake build configuration.
8. `README.md`: project overview.

## Requirements

- CMake (>= 3.16 recommended).
- A C++17 compiler (`clang++`, `g++`, or MSVC).
- Raylib.

## Instructions

### Installation

Make sure you have a C++17 compiler and CMake installed.

Install Raylib:
- macOS (Homebrew): `brew install raylib`

### Configure & Build

```bash
cmake -S . -B build
cmake --build build
```

Output:
- Linux/macOS: `bin/game`
- Windows: `bin/game.exe`

### Run

```bash
./bin/game
```

(Alternative)

```bash
cmake --build build --target run
```

### Clean

Delete the build output directories:
- `build/`
- `bin/`

GUI is always enabled. There is no CLI runtime mode.
